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
#ifndef __VECTOR_HEADER_INCLUDED_
#define __VECTOR_HEADER_INCLUDED_

#include <iostream>
#include <cassert>
#include "myAlgorithm.hpp"

namespace fmi {
namespace sdp {

template<class DataType>
class Vector {
public:

#include "vectorProxy.hpp"
	friend class VectorProxy;

	typedef bool(*pointerToBool)(const DataType&, const DataType&);

	Vector(size_t capacity= 10, float step = 1.5)
		:vector(nullptr)
		, capacity(capacity)
		, size(0)
		, sortByFunction(nullptr){

		if (this->capacity) this->vector = new DataType[this->capacity];

		this->setStep(step);
	}

	~Vector() {
		this->clean();
	}

	Vector(const Vector<DataType> &other) {
		this->copyFrom(other);
	}

	template<class OtherType>
	Vector(const Vector<OtherType> &other) {
		this->copyFrom(other);
	}

	Vector& operator=(const Vector<DataType> &other) {
		if (this != &other) {
			this->clean();
			this->copyFrom(other);
		}
		return *this;
	}

	template<class OtherType>
	Vector& operator=(const Vector<OtherType> &other) {
		if (this != &other) {
			this->clean();
			this->copyFrom(other);
		}
		return *this;
	}

	VectorProxy operator[](size_t index) {
		assert(index < this->size);

		return VectorProxy(*this,index);
	}

	const DataType& operator[](size_t index) const {
		assert(index < this->size);

		return this->vector[index];
	}

	void sort(pointerToBool p){
		if (this->sortByFunction != p) {
			this->sortByFunction = p;

			if (this->size > 1) {
				if (sizeof(DataType) > 16) {
					selectSort(this->vector, this->size, this->sortByFunction);
				}

				DataType *helpArr = new(std::nothrow) DataType[this->size];

				if (helpArr == nullptr) quickSort(this->vector, this->size, this->sortByFunction);
				else					mergeSort(this->vector, this->size, helpArr, this->sortByFunction);

				delete[] helpArr;
			}
		}
	}
	
	size_t search(const DataType &value, bool searchBySorted = true) const {
		if (this->size) {
			if (searchBySorted && this->sortByFunction) 
				return binarySearch(this->vector, this->size,value, this->sortByFunction);

			for (size_t i = 0; i < this->size; ++i) 
				if (value == this->vector[i]) return i;
		}

		return this->size;//invalid position
	}

	//returns the index of new data
	size_t insertInSorted(const DataType &value) {

		if (this->sortByFunction) {
			size_t i = this->size - 1;
			while (this->sortByFunction(value, this->vector[i])) --i;
			this->insert(i + 1, value);
			return i + 1;
		}

		else {
			this->pushBack(value);
			return this->size - 1;
		}
	}

	void notSorted() {
		this->sortByFunction = nullptr;
	}

	bool isSorted() const {
		return (this->sortByFunction != nullptr);
	}

	void pushBack(const DataType &value) {
		this->insert(this->size, value);
	}

	DataType popBack(){
		return this->vector[--this->size];
	}

	void insert(size_t pos, const DataType &value) {
		if (this->size == this->capacity) this->resize(this->step*this->capacity);

		//when vector is sorted
		if (this->sortByFunction) {
			//check - position on the new element keep sorted vector
			if ((pos && (this->sortByFunction(pos - 1, value) == false))
				|| (pos != this->size && (this->sortByFunction(value, pos) == false)))
				
				this->sortByFunction = nullptr;
		}

		if (pos >= this->size) this->vector[this->size] = value;
		else {
			for (unsigned i = this->size; i > pos; --i) this->vector[i] = this->vector[i - 1];

			this->vector[pos] = value;
		}

		++this->size;
	}

	DataType erase(size_t index) {
		assert(index < this->size);

		DataType value = this->vector[index];
		--this->size;

		while (index < this->size) this->vector[index] = this->vector[++index];

		return value;
	}

	void erase(size_t left, size_t right) {
		assert(right < this->size);

		this->size -= (right - left + 1);

		for (size_t i = 0,; (left+i <= right) && (left+i<this->size); ++i) {
			this->vector[left + i] = this->vector[right + i];
		}
	}

	void resize(size_t capacity) {
		if (capacity < this->size)this->size = capacity;
		this->capacity = capacity;

		DataType *p = new DataType[this->capacity];

		for (size_t i = 0; i < this->size; ++i) p[i] = this->vector[i];

		delete[] this->vector;
		this->vector = p;
	}

	size_t getSize() const {
		return this->size;
	}

	bool isEmpty() const {
		return !this->size;
	}

	void setStep(float step) {
		if (step <= 1.0) return;

		this->step = step;
	}

private:
	DataType *vector;
	size_t size;
	size_t capacity;
	float step;
	pointerToBool sortByFunction;

	template<class OtherType>
	void copyFrom(const Vector<OtherType> &other) {
		this->step = other.step;
		this->sortByFunction = other.sortByFunction;
		this->size = other.size;
		this->capacity = other.capacity;

		this->vector = new DataType[this->capacity];

		for (size_t i = 0; i < this->size; ++i) this->vector[i] = other.vector[i];
	}

	void clean() {
		delete[] this->vector;
	}

	void move(size_t index) {
		assert(index < this->size);
		if (this->sortByFunction) {
			DataType value(this->vector[index]);
			size_t newPosition = index;

			while (newPosition &&
				this->sortByFunction(this->vector[index], this->vector[newPosition-1])) {
				--newPosition;
			}

			if (newPosition != index){
				for (; index > newPosition; --index)
					this->vector[index] = this->vector[index - 1];

				this->vector[newPosition] = value;
				return;
			}

			while (newPosition<this->size-1 &&
				this->sortByFunction(this->vector[newPosition + 1], this->vector[index] )) {
				++newPosition;
			}

			if (newPosition != index){
				for (; index < newPosition; ++index)
					this->vector[index] = this->vector[index + 1];

				this->vector[newPosition] = value;
				return;
			}
		}
	}
};

}
}

#endif //__VECTOR_HEADER_INCLUDED_