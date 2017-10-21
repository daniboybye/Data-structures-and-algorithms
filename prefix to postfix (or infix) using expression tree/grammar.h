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
#pragma once

#ifndef __GRAMMAR_HEADER_UNCLUDED_
#define __GRAMMAR_HEADER_UNCLUDED_

class Grammar 
{
public:

	struct Operator 
	{
		Operator()
			:flag(false) {}

		Operator(unsigned char symbol,unsigned char effect,unsigned char associativity)
			:symbol(symbol)
			,effect(effect)
			,associativity(associativity)
			,flag(true) {}

		unsigned char symbol, effect, associativity;
		char flag;
	};

	explicit Grammar(const char*);
	Grammar(const Grammar&) = default;
	Grammar& operator=(const Grammar&) = default;
	~Grammar() = default;

	const Operator* get_operator(const unsigned char symbol) const
	{
		return (this->operators[symbol].flag ? &this->operators[symbol] : nullptr);
	}

private:
	Operator operators[256];
};

#endif //__GRAMMAR_HEADER_UNCLUDED_