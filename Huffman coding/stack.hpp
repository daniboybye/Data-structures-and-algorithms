/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Daniel Urumov
* @idnumber 45135
* @task 0
* @compiler VC
*
*/
#ifndef _STACK_HEADER_INCLUDED_
#define _STACK_HEADER_INCLUDED_

#include "linkedList.h"

namespace fmi {
namespace sdp {
namespace structures {


template <class T>
class Stack {
public:
	Stack() : list() {}

	Stack(const Stack & other)
	: list(other.list) {}

	Stack& operator=(const Stack & other) {
		if (this != &other) {
			this->list.operator=(other.list);
		}
		return *this;
	}

	unsigned getSize() const {
		return this->list.getSize();
	}
	T pop() {
		return this->list.popFront();
	}
	void push(const T & element) {
		this->list.pushFront(element);
	}
	T& front() const {
		return this->list.front();
	}

	bool isEmpty() const{
		return this->list.isEmpty();
	}

	bool isFull() const {
		return false;
	}

private:
	LinkedList<T> list;
};

}//namespace structures
}//namespace sdp
}//namespace fmi

#endif //_STACK_HEADER_INCLUDED_