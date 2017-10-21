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
#include "compose.h"

#include <iomanip>
#include <iostream>

int menu(const char * str1,const char * str2) 
{
	try
	{
		auto language = std::make_unique<Language>(str2, str1);
		
		language->pre_print(std::cout);
		std::cout << std::endl;
		language->post_print(std::cout);
		std::cout << std::endl;
		language->in_print(std::cout);
		std::cout << std::endl<<std::endl;

		std::cout << std::fixed << std::setprecision(5) << language->calculate() << std::endl;
	}
	catch (const std::ios_base::failure & exception) 
	{
		std::cout << exception.what() << std::endl;
		return -1;
	}
	catch (const char * str) 
	{
		//изразът не е валиден
		std::cout << str <<std::endl;
	}
	return 0;
}

void init()
{
	Function::add_func('+', [](double lhs, double rhs) { return lhs + rhs; });
	Function::add_func('-', [](double lhs, double rhs) { return lhs - rhs; });
	Function::add_func('*', [](double lhs, double rhs) { return lhs * rhs; });
	Function::add_func('/', [](double lhs, double rhs) 
	{
		if(std::fabs(rhs)<1e-10) throw "Division on zero";
		return lhs / rhs; 
	});
	Function::add_func('^', [](double lhs, double rhs) { return std::pow(lhs,rhs); });
}

int main(int argc,char * argv[])
{
	if (argc != 3) return -1;

	init();

	return menu(argv[1], argv[2]);
}