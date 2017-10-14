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

#include "bitVector.hpp"

using namespace fmi::sdp::bitStructures;

BitVector::BitVector(size_t capacity)
: size(0)
, capacity(capacity-(capacity&7)) {

	this->vector = (char*)malloc(this->capacity >> 3);

	if (this->vector == nullptr)
		throw std::bad_alloc();

	memset(this->vector, 0, this->capacity >> 3);
}

BitVector::BitVector(const char *str, size_t lenStr, size_t capacity) {
	this->vector = (char*)malloc(capacity >> 3);

	if (this->vector == nullptr)
		throw std::bad_alloc();

	memcpy(this->vector, str, lenStr);
	memset(this->vector + lenStr, 0, (capacity >> 3) - lenStr);
	this->size = (lenStr << 3);
}

BitVector::BitVector(const char *str, size_t lenStr)
:BitVector(str, lenStr, lenStr << 3)
{}


BitVector::BitVector(const char *str)
:BitVector(str, strlen(str))
{}

BitVector::BitVector(const BitVector &other)
:size(other.size)
, capacity(other.capacity) {
	this->copyFrom(other);
}

BitVector& BitVector::operator=(const BitVector &other) {
	if (this != &other) {
		this->size = other.size;
		this->capacity = other.capacity;
		free(this->vector);
		this->copyFrom(other);
	}

	return *this;
}

BitVector::~BitVector() {
	free(this->vector);
}

bool BitVector::setSize(size_t size) {
	if (size > this->capacity && this->resize() == false)
		return false;

	this->size = size;
	return true;
}

bool BitVector::appendBitVector(const BitVector &other) {
	while (this->size + other.size > this->capacity) {
		if (this->resize() == false)
			return false;
	}

	for (size_t i = 0; i < other.size; ++i) {
		if (other.getElement(i))
			this->vector[this->size >> 3] |= (1 << (this->size & 7));

		++this->size;
	}

	return true;
}

bool BitVector::appendReverseBitVector(const BitVector &other) {
	while (this->size + other.size > this->capacity) {
		if (this->resize() == false)
			return false;
	}

	size_t i = other.size;
	while (--i) {
		if (other.getElement(i))
			this->vector[this->size >> 3] |= (1 << (this->size & 7));

		++this->size;
	}

	return true;
}

void BitVector::modified(size_t index, bool value) {
	if (this->getElement(index) == value)
		return;

	if (value)
		this->vector[index >> 3] |= (1 << (index & 7));
	else
		this->vector[index >> 3] &= (~(1 << (index & 7)));
}

void BitVector::insert(size_t pos, const char *str, size_t lenStr) {
	memcpy(this->vector + (pos >> 3), str, lenStr);

	lenStr <<= 3;
	lenStr += pos;
	if (lenStr > this->size)
		this->size = lenStr;
}

bool BitVector::resize() {
	if (this->capacity > 16e7)
		return false;

	this->capacity <<= 1;

	char *p = (char*)realloc(this->vector, this->capacity >> 3);

	if (p == nullptr) {
		this->capacity >>= 1;
		return false;
	}

	this->vector = p;
	
	memset(this->vector + (this->capacity >> 4), 0, (this->capacity >> 3) - (this->capacity >> 4));

	return true;
}

void BitVector::copyFrom(const BitVector &other) {
	this->vector = (char*)malloc(this->capacity >> 3);
	memcpy(this->vector, other.vector, this->capacity >> 3);
}