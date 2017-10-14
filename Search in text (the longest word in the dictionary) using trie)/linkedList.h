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
#ifndef __LINKED_LIST_HEADER_INCLUDED_
#define __LINKED_LIST_HEADER_INCLUDED_

#include <iostream>

namespace fmi {
namespace sdp {

template<class T>
class LinkedList {
public:
	struct Note {
		T data;
		Note * pNext;
		Note(const T & other) {
			data = other;
			pNext = nullptr;
		}
		Note(const T & other, Note * p) {
			data = other;
			pNext = p;
		}
	};

	LinkedList();
	LinkedList(const LinkedList&);
	template<class OtherType>
	LinkedList(const LinkedList<OtherType>&);
	LinkedList& operator=(const LinkedList&);
	template<class OtherType>
	LinkedList& operator=(const LinkedList<OtherType>&);
	~LinkedList();

	unsigned getSize() const;
	bool isEmpty() const;
	T& front() const;
	T& back() const;
	T& at(unsigned) const;

	void move(LinkedList<T>&);
	void pushFront(const T&);
	void pushBack(const T&);
	T popFront();
	T popBack();
	void insert(unsigned,const T&);
	T erase(unsigned);
	void erase(unsigned,unsigned);
	void clear();

	#include "iterator.hpp"

	Iterator getIterator() const {
		return Iterator(this->head);
	}

protected:
	//Note* getNote(unsigned) const;
	Note* getNote(unsigned index) const {
		if (index >= this->size) return nullptr;
		else if (index == this->size - 1) return this->tail;

		Note * returnValue = this->head;
		for (unsigned i = 0; i < index; ++i) {
			returnValue = returnValue->pNext;
		}

		return returnValue;
	}

private:
	unsigned size;
	Note * head;
	Note * tail;

	void clean();
	template<class OtherType>
	void copyFrom(const LinkedList<OtherType>&);
};

#include "linkedList.hpp"

}
}


#endif // __LINKED_LIST_HEADER_INCLUDED_