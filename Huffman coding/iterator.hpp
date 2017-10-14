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
#ifndef __ITERATOR_LINKED_LIST_INCLUDED_
#define __ITERATOR_LINKED_LIST_INCLUDED_

class Iterator {
	friend class LinkedList;

public:
	const T& operator*() const {
		return this->pointer->data;
	}

	T& operator*() {
		return this->pointer->data;
	}

	const T* operator->() const {
		return &(this->pointer->data);
	}

	T* operator->() {
		return &(this->pointer->data);
	}

	Iterator& operator++() {
		if (this->pointer != nullptr) {
			this->pointer = this->pointer->pNext;
		}
		return *this;
	}

	Iterator operator++(int)
	{
		Iterator res(*this);
		++(*this);
		return res;
	}

	operator bool() const {
		return (this->pointer == nullptr) ? false : true;
	}

private:
	Note *pointer;
	
	Iterator(Note *list) : pointer(list) {}

	void operator=(const Iterator&) = delete;
};

#endif //__ITERATOR_LINKED_LIST_INCLUDED_