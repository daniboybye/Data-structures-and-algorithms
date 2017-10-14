/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Daniel Urumov
* @idnumber 45135
* @task 3
* @compiler VC
*
*/
#ifndef __QUEUE_HEADER_INCLUDED_
#define __QUEUE_HEADER_INCLUDED_

#include "linkedList.h"

namespace fmi {
namespace sdp {

template<class T>
class Queue {
public:
	Queue() :list() {}
	Queue(const Queue & other)
		: list(other.list) {}
	Queue& operator=(const Queue & other) {
		if (this != &other) {
			this->list.operator=(other.list);
		}

		return *this;
	}
	~Queue() {};

	void enqueue(const T & value) {
		this->list.pushBack(value);
	}
	T dequeue() {
		return this->list.popFront();
	}
	unsigned getSize() const {
		return this->list.getSize();
	}
		
	T& front() const {
		return this->list.front();
	}

	bool isEmpty() const {
		return this->list.isEmpty();
	}

private:
	LinkedList<T> list;
};

}
}

#endif //__QUEUE_HEADER_INCLUDED_