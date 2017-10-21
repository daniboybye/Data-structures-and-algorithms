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
#include "grammar.h"
#include "stream_ptr.hpp"
#include <cstring>

Grammar::Grammar(const char * file_name) 
{
	unique_ifstream file(new std::ifstream(file_name));

	if (file->good() == false) 
	{
		char str[1024] = { "File is not open! File's name is " };
		
		throw std::ios_base::failure(strcat(str, file_name));
	}
	
	unsigned char symbol, effect, associativity;
	while (true) 
	{
		*file >> symbol >> effect >> associativity;

		if (file->good() == false) break;

		this->operators[symbol] = Operator(symbol,effect,associativity);
	}
}