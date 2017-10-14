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

#pragma once

#ifndef __BIT_QUEUE_HEADER_INCLUED_
#define __BIT_QUEUE_HEADER_INCLUED_

#include "bitVector.hpp"

namespace fmi {
namespace sdp {
namespace bitStructures {


class BitQueue
	: protected BitVector {

public:
	BitQueue(size_t capacity);

	BitQueue(const char *str, size_t size);

	BitQueue(const char *str);

	BitQueue(const BitQueue&) = default;

	BitQueue(const char *str, size_t size, size_t capacity);

	BitQueue& operator=(const BitQueue&) = default;

	~BitQueue() = default;

	bool front() const {
		return this->getElement(this->begin);
	}

	size_t getSize() const {
		return this->size;
	}

	size_t getCapacity() const {
		return this->BitVector::getCapacity();
	}

	bool dequeue();

	void clear() {
		this->BitVector::clear();

		this->begin = this->end = this->size = 0;
	}

	void enqueue(const char *str, size_t lenStr);

private:
	size_t begin, end, size;
};

}//namespace bitStructures
}//namespace sdp
}//namespace fmi


#endif //__BIT_QUEUE_HEADER_INCLUED_