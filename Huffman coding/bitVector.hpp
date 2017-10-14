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

#ifndef __BIT_VECTOR_HEADER_INCLUDED_
#define __BIT_VECTOR_HEADER_INCLUDED_

#include <cstdint>
#include <vector>
#include <malloc.h>
#include <cassert>
#include <cstring>


namespace fmi {
namespace sdp {
namespace bitStructures {



class BitVector {
public:
	BitVector(size_t capacity = 64);

	BitVector(const char *str, size_t lenStr, size_t capacity);

	BitVector(const char *str, size_t lenStr);

	BitVector(const char *str);

	BitVector(const BitVector &other);

	BitVector& operator=(const BitVector &other);

	~BitVector();

	bool setSize(size_t size);

	operator char*() const {
		return this->vector;
	}

	size_t getSize() const {
		return this->size;
	}

	size_t getCapacity() const {
		return this->capacity;
	}

	template<typename Iterator>
	bool append(Iterator ref1,Iterator ref2, size_t size) {

		while (size + this->size > this->capacity) {
			if (this->resize() == false)
				return false;
		}

		while (ref1 != ref2) {
			if (*ref1++)
				this->vector[this->size >> 3] |= (1 << (this->size & 7));

			++this->size;
		}

		return true;
	}

	template<typename DataType>
	bool push_back(DataType value) {
		if (this->size == this->capacity && this->resize() == false)
			return false;

		if (value)
			this->vector[this->size >> 3] |= (1 << (this->size & 7));

		++this->size;
		return true;
	}

	bool appendBitVector(const BitVector &other);

	bool appendReverseBitVector(const BitVector &other);

	void clear() {
		memset(this->vector, 0, this->capacity >> 3);
		this->size = 0;
	}

	void memsetWithNull() {
		memset(this->vector, 0, this->capacity >> 3);
	}

	void memsetWithOne() {
		memset(this->vector, -1, this->size >> 3);
		
		this->vector[this->size >> 3] |= (this->size & 7);
	}

	bool getElement(size_t index) const {
		assert(index < this->size);

		return ((this->vector[index >> 3] >> (index & 7)) & 1);
	}

	void modified(size_t index, bool value);

protected:
	void insert(size_t pos, const char *str, size_t lenStr);

private:
	char *vector;
	size_t size, capacity;

	bool resize();

	void copyFrom(const BitVector &other);
};

}//namespace bitStructures
}//namespace sdp
}//namespace fmi




#endif //__BIT_VECTOR_HEADER_INCLUDED_