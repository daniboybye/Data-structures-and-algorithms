/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Daniel Urumov
* @idnumber 45135
* @task 1
* @compiler VC
*
*/
#include "compose.h"
#include "stream_ptr.hpp"

#include <cstring>

Function::pF Function::map[256] = { nullptr, };

#define CURR_SIZE 2

std::unique_ptr<Compose> Factory::Create(const char * file_name,const Grammar & grammar) 
{
	unique_ifstream file(new std::ifstream(file_name));
	if (file->good() == false) 
	{	
		char str[10000] = { "File is not open! File's name is " };
		throw std::ios_base::failure(strcat(str, file_name));
	}

	std::unique_ptr<Compose> return_value(Rec(*file, grammar).step_one());

	if (file->is_open()&&file->good()) 
	{
		//изчитам файла до края
		char curr[CURR_SIZE];
		file->get(curr, CURR_SIZE);

		while (file->good() && (curr[0] == ' ' || curr[0] == '\n' || curr[0] == '\0' || curr[0] == '\t'))
			file->get(curr, CURR_SIZE);
		
		//ако има символи, които не съм прочел
		if (file->good()) throw "Error";
	}
	else if (return_value == nullptr)
		throw "Error";

	return return_value;
}

std::unique_ptr<Compose> Factory::Rec::step_one()
{
	while (file.good()&&file.is_open()) 
	{
		curr = file.peek();

		if (curr == '\0' || curr == '\n') throw "Error";

		else if (curr == ' ' || curr == '\t') 
		{
			file.seekg(1, std::ios::cur);
			continue;
		}

		else if (oper = grammar.get_operator(curr)) 
			return read_function();

		else if ((curr >= '0' && curr <= '9') || curr == '-' || curr == '+') 
		{
			if (curr == '-' || curr == '+') file.seekg(1, std::ios::cur);

			return read_number();
		}

		else throw "Error";
	}
	return nullptr;
}

std::unique_ptr<Compose> Factory::Rec::read_number() 
{
	double flag_signed = 1;

	if (curr == '-' || curr == '+') {
		if (curr == '-') flag_signed = -1;

		curr = file.peek();

		if (curr<'0' || curr>'9') throw "Error";
	}

	double number;
	file >> number;

	number *= flag_signed;

	if (file.eof()) file.close();

	return std::make_unique<Number>(number);
}

std::unique_ptr<Compose> Factory::Rec::read_function() 
{
	file.seekg(1, std::ios::cur);

	char help = file.peek();

	//ако - или + са оператори, трябва да проверя да ли са унарни
	if ((curr == '-' || curr == '+') && (help >= '0'&& help <='9'))
		return read_number();
	
	else if (help != ' '&&help != '\t') throw "Error";
	
	auto const op = oper;

	auto left = step_one();
	if (left == nullptr) throw "Error";

	auto right = step_one();
	if (right == nullptr) throw "Error";

	return std::make_unique<Function>(op,left,right);
}