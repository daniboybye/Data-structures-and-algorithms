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

#include "bitQueue.h"

using namespace fmi::sdp::bitStructures;

BitQueue::BitQueue(size_t capacity)
: BitVector(capacity)
, begin(0)
, end(0)
, size(0)
{}

BitQueue::BitQueue(const char *str, size_t size)
: BitVector(str, size)
, begin(0)
, end(size << 3)
, size(size << 3)
{}

BitQueue::BitQueue(const char *str)
: BitQueue(str, strlen(str))
{}

BitQueue::BitQueue(const char *str, size_t size, size_t capacity)
: BitVector(str, size, capacity)
, end(size<<3)
, begin(0)
, size(size<<3)
{}

bool BitQueue::dequeue() {
	assert(this->size);

	bool value = this->getElement(this->begin);
	++this->begin;
	this->begin %= this->BitVector::getCapacity();
	--this->size;

	return value;
}

void BitQueue::enqueue(const char *str, size_t lenStr) {
	assert((lenStr << 3) <= this->BitVector::getCapacity() - this->size);

	this->size += (lenStr << 3);

	if (this->begin < this->end){
		size_t len = this->BitVector::getCapacity() - this->end;
		len >>= 3;

		if (len >= lenStr) {
			this->insert(this->end, str, lenStr);

			this->end += (lenStr << 3);
		}
		else {
			this->insert(this->end, str, len);

			str += len;
			lenStr -= len;

			this->insert(0, str, lenStr);

			this->end = (lenStr << 3);
		}

	}
	else{
		this->insert(this->end, str, lenStr);
		this->end += (lenStr << 3);
	}
}