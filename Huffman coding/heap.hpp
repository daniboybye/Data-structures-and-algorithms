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
#ifndef __HEAP_HEADER_INCLUED_
#define __HEAP_HEADER_INCLUED_

#include <vector>
#include <algorithm>

namespace fmi {
namespace sdp {
namespace structures {

template<class DataType>
class Heap {
public:
	Heap()
		:vector()
	{}

	Heap(const Heap &other)
		:vector(vector)
	{}

	Heap& operator=(const Heap &other) {
		if (this != &ohter)
			this->vector = other.vector;

		return *this;
	}

	Heap(DataType *arr, size_t size)
		:Heap() {

		const DataType *end = arr + size;

		while (arr != end)
			this->vector.push_back(*arr++);

		this->create();
	}

	Heap(const std::vector<DataType> &vector)
		:Heap() {

		this->vector.insert(this->vector.begin(), vector.begin(), vector.end());

		this->create();
	}

	~Heap() {}

	size_t getSize() const {
		return this->vector.size();
	}

	const DataType& top() const {
		return this->vector[0];
	}

	DataType pop() {
		DataType value = this->vector[0];

		this->vector[0] = this->vector.back();
		this->vector.pop_back();

		this->pushDown(0);

		return value;
	}

	void push(const DataType& value) {
		this->vector.push_back(value);

		this->pullUp(this->vector.size() - 1);
	}

	bool isEmpty() const {
		return this->vector.empty();
	}

private:
	std::vector<DataType> vector;

	void pullUp(size_t index) {
		size_t parent;

		while (index) {
			parent = ((index - 1) >> 1);

			if (this->vector[index] < this->vector[parent]){
				std::swap(this->vector[index], this->vector[parent]);
				index = parent;
			}
			else {
				break;
			}
		}
	}

	void pushDown(size_t index) {
		size_t child = (index << 1) + 1;

		while (child < this->vector.size()) {

			if (child + 1 != this->vector.size() &&
				this->vector[child + 1] < this->vector[child])
				++child;

			if (this->vector[index] < this->vector[child])
				break;

			std::swap(this->vector[index], this->vector[child]);
			index = child;

			child = (index << 1) + 1;
		}
	}

	void create() {

		size_t index = this->vector.size();

		while (index)
			this->pushDown(--index);

	}
};

template<class DataType>
class Heap<DataType*> {
public:
	Heap()
		:vector()
	{}

	Heap(const Heap &other)
		:vector(vector)
	{}

	Heap& operator=(const Heap &other) {
		if (this != &ohter)
			this->vector = other.vector;

		return *this;
	}

	Heap(DataType **arr, size_t size)
		:Heap() {

		const DataType *end = arr + size;

		while (arr != end)
			this->vector.push_back(*arr++);

		this->create();
	}

	Heap(const std::vector<DataType*> &vector)
		:Heap() {

		this->vector.insert(this->vector.begin(), vector.begin(), vector.end());

		this->create();
	}

	~Heap() {}

	size_t getSize() const {
		return this->vector.size();
	}

	DataType* top() const {
		return this->vector[0];
	}

	DataType* pop() {
		DataType* value = this->vector[0];

		this->vector[0] = this->vector.back();
		this->vector.pop_back();

		this->pushDown(0);

		return value;
	}

	void push(DataType* value) {
		this->vector.push_back(value);

		this->pullUp(this->vector.size() - 1);
	}

	bool isEmpty() const {
		return this->vector.empty();
	}

private:
	std::vector<DataType*> vector;

	void pullUp(size_t index) {
		size_t parent;

		while (index) {
			parent = ((index - 1) >> 1);

			if (*this->vector[index] < *this->vector[parent]){
				std::swap(this->vector[index], this->vector[parent]);
				index = parent;
			}
			else {
				break;
			}
		}
	}

	void pushDown(size_t index) {
		size_t child = (index << 1) + 1;

		while (child < this->vector.size()) {

			if (child + 1 != this->vector.size() &&
				*this->vector[child + 1] < *this->vector[child])
				++child;

			if (*this->vector[index] < *this->vector[child])
				break;

			std::swap(this->vector[index], this->vector[child]);
			index = child;

			child = (index << 1) + 1;
		}
	}

	void create() {

		size_t index = this->vector.size();

		while (index)
			this->pushDown(--index);

	}
};

}//namespace structures
}//namespace sdp
}//namespace fmi

#endif //__HEAP_HEADER_INCLUED_