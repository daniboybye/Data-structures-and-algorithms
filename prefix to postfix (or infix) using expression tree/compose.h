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

#ifndef __COMPOSE_NUMBER_OPERATION_UNCLUDED_
#define __COMPOSE_NUMBER_OPERATION_UNCLUDED_

#include "grammar.h"

#include <ostream>
#include <memory>

class Interface 
{
public:
	virtual ~Interface() = default;
	virtual void pre_print(std::ostream&) const = 0;
	virtual void in_print(std::ostream&) const = 0;
	virtual void post_print(std::ostream&) const = 0;
	virtual double calculate() const = 0;
};

class Compose :public Interface 
{
public:
	virtual ~Compose() override = default;
	virtual void pre_print(std::ostream&) const override = 0 ;
	virtual void in_print(std::ostream&) const override = 0;
	virtual void post_print(std::ostream&) const override = 0 ;
	virtual double calculate() const override = 0;
};

class Number: public Compose 
{
public:
	Number() : number(0.0) {}

	explicit Number(double n)
		:number(n) {}

	Number(const Number&) = default;
	virtual ~Number() override = default;
	virtual void pre_print(std::ostream & os) const override
	{
		os << this->number;
	}
	virtual void in_print(std::ostream & os) const override
	{
		os << this->number;
	}
	virtual void post_print(std::ostream & os) const override
	{
		os << this->number;
	}
	virtual double calculate() const override
	{
		return this->number;
	}

private:
	double number;
};

class Function : public Compose 
{
public:
	Function(const Grammar::Operator * function,
			std::unique_ptr<Compose>& left,
			std::unique_ptr<Compose>& right)
		: function(function)
		, left(std::move(left))
		, right(std::move(right))
	{}

	Function(const Function&) = delete;
	void operator=(const Function&) = delete;
	virtual~Function() override = default;
	
	virtual void pre_print(std::ostream & os) const override
	{
		os << this->function->symbol << " ";
		this->left->pre_print(os);
		os << " ";
		this->right->pre_print(os);
	}

	virtual void in_print(std::ostream & os) const override
	{
		os << "(";
		this->left->in_print(os);
		os << " " << this->function->symbol<<" ";
		this->right->in_print(os);
		os << ")";
	}

	virtual void post_print(std::ostream & os) const override
	{
		this->left->post_print(os);
		os << " ";
		this->right->post_print(os);
		os << " " << this->function->symbol;
	}

	void set_left_argument(Compose * value)
	{
		this->left.reset(value);
	}
	void set_right_argument(Compose * value)
	{
		this->right.reset(value);
	}

	typedef double(*pF)(double, double);

private:
	static pF map[256];

public:
	virtual double calculate() const override
	{
		return Function::map[this->function->effect](this->left->calculate(),
								this->right->calculate());
	}
	static void add_func(unsigned char symbol, pF func)
	{
		Function::map[symbol] = func;
	}

private:
	const Grammar::Operator * function;
	std::unique_ptr<Compose> left,right;
};

class Factory
{
public:
	Factory()=delete;
	Factory(const Factory&)=delete;
	void operator=(const Factory&) = delete;
	~Factory() = delete;

	static std::unique_ptr<Compose> Create(const char*,const Grammar&);

private:

	struct Rec
	{
		Rec(std::ifstream & file,const Grammar & grammar)
			:file(file)
			,grammar(grammar)
		{}

		std::ifstream & file;
		const Grammar & grammar;
		const Grammar::Operator* oper;
		char curr;

		std::unique_ptr<Compose> step_one();
		std::unique_ptr<Compose> read_number();
		std::unique_ptr<Compose> read_function();
	};
};

class Language :public Interface 
{
public:
	Language(const char * grammar_file_name, const char * file_name)
		: grammar(std::make_unique<Grammar>(grammar_file_name))
		, root(Factory::Create(file_name, *this->grammar))
	{}
	virtual ~Language() override = default;
	virtual void pre_print(std::ostream & os) const override
	{
		this->root->pre_print(os);
	}
	virtual void in_print(std::ostream & os) const override
	{
		this->root->in_print(os);
	}
	virtual void post_print(std::ostream & os) const override
	{
		this->root->post_print(os);
	}
	virtual double calculate() const override
	{
		return this->root->calculate();
	}

private:
	std::unique_ptr<Grammar> grammar;
	std::unique_ptr<Compose> root;
};

#endif //__COMPOSE_NUMBER_OPERATION_UNCLUDED_