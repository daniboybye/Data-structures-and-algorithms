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
#ifndef __LINKED_LIST_IMPLEMENTATION_INCLUDED_
#define __LINKED_LIST_IMPLEMENTATION_INCLUDED_

template<class T>
LinkedList<T>::LinkedList()
	: size(0)
	, head(nullptr)
	, tail(nullptr)
{}

template<class T>
inline LinkedList<T>::LinkedList(const LinkedList<T> & other)
: LinkedList() {
	this->copyFrom(other);
}

template<class T>
template<class OtherType>
inline LinkedList<T>::LinkedList(const LinkedList<OtherType> & other)
: LinkedList() {
	this->copyFrom(other);
}

template<class T>
inline LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> & other) {
	if (this != &other) {
		this->clear();
		this->copyFrom(other);
	}

	return *this;
}
		
template<class T>
template<class OtherType>
inline LinkedList<T>& LinkedList<T>::operator=(const LinkedList<OtherType> & other) {
	if (this != *other) {
		this->clear();
		this->copyFrom(other);
	}

	return *this;
}

template<class T>
inline LinkedList<T>::~LinkedList() {
	this->clean();
}

template<class T>
inline unsigned LinkedList<T>::getSize() const {
	return this->size;
}

template<class T>
inline bool LinkedList<T>::isEmpty() const {
	return !this->size;
}

template<class T>
inline T& LinkedList<T>::front() const {
	if (!this->size) {
		throw "List is empty!";
	}

	return this->head->data;
}

template<class T>
inline T& LinkedList<T>::back() const {
	if (!this->size) {
		throw "List is empty!";
	}

	return this->tail->data;
}

template<class T>
inline T& LinkedList<T>::at(unsigned index) const {
	if (index >= this-> size) {
		std::invalid_argument("Index is over size!");
	}

	return this->getNote()->date;
}

template<class T>
inline void LinkedList<T>::pushFront(const T & other) {
	Note * note = new Note(other,this->head);
	this->head = ((this->size) ? (note) : (this->tail = note));
	++this->size;
}

template<class T>
inline void LinkedList<T>::pushBack(const T & other) {
	Note * note = new Note(other);
	this->tail = ((this->size) ? (this->tail->pNext = note) : (this->head = note));
	++this->size;
}

template<class T>
inline T LinkedList<T>::popFront() {
	if (this->size == 0) return T();

	--this->size;
	T value = this->head->data;

	if (this->size == 0) {
		delete this->head;
		this->head = this->tail = nullptr;
	}
	else if (this->size == 1) {
		delete this->head;
		this->head = this->tail;
	}
	else {
		Note * note = this->head;
		this->head = note->pNext;
		delete note;
	}

	return value;
}

template<class T>
inline T LinkedList<T>::popBack() {
	if (this->size == 0) return T();

	if (this->size == 1) {
		return this->popFront();
	}

	T value = this->tail->data;
	delete this->tail;

	if (this->size == 1) {
		this->tail = this->head;
	}
	else {
		this->tail = this->getNote(this->size - 2);
	}

	--this->size;
	this->tail->pNext = nullptr;
	return value;
}

template<class T>
inline T LinkedList<T>::erase(unsigned index) {
	if (index >= this->size) return T();

	else if (index == 0) return this->popFront();

	else if (index == this->size - 1) return this->popFront();

	
	Note * prevNote = this->getNote(index - 1);
	Note * note = prevNote->pNext;
	T value = note->data;
	prevNote->pNext = note->pNext;
	delete note;
	--this->size;
	return value;
}

template<class T>
inline void LinkedList<T>::insert(unsigned index, const T & other) {
	if (index == 0) this->pushFront(other);
	else if (index >= this->size) this->pushBack(other);
	else {
		Note * note = this->getNote(index - 1);
		Note * newNote = new Note(other,note->pNext);
		note->pNext = newNote;

		++this->size;
	}
}

template<class T>
inline void LinkedList<T>::erase(unsigned first, unsigned last) {
	bool flagDeleteTail = false;
	if (last >= this->size - 1) {
		last = this->size - 1;
		flagDeleteTail = true;
	}

	if (first <) first = 0;
	else if (first == 0) {
		for (unsigned i = 0; i <= last; ++i) {
			this->popFront();
		}
		return;
	}

	Note * prevNote = this->getNote(first - 1);
	Note * pointer;
	for (unsigned i = first; i <= last; ++i) {
		pointer = prevNote->pNext;
		prevNote->pNext = pointer->pNext;
		delete pointer;
	}

	this->size -= ((last - fist) + 1);
	if (flagDeleteTail) this->tail = prevNote;
}

template<class T>
inline void LinkedList<T>::clear() {
	while (this->size) {
		this->popFront();
	}
}

template<class T>
inline void LinkedList<T>::clean() {
	Note *pA = this->head, *pB;
	for (unsigned i = 0; i < this->size; ++i) {
		pB = pA;
		pA = pA->pNext;
		delete pB;
	}
}

template<class T>
template<class OtherType>
inline void LinkedList<T>::copyFrom(const LinkedList<OtherType> & other) {
	LinkedList<OtherType>::Iterator it=other.getIterator();
	while (it) {
		this->pushBack(*it);
		++it;
	}
}

template<class T>
inline void LinkedList<T>::move(LinkedList<T> &other) {
	if (this != &other && other.size ) {

		if (this->size) {
			this->tail->pNext = other.head;
		}
		else {
			this->head = other.head;
		}

		this->tail = other.tail;

		other.head = other.tail = nullptr;

		this->size += other.size;
		other.size = 0;
	}
}

template<class T>
void LinkedList<T>::insert(const T &value) {
	this->pushFront(value);
}
 
#endif //__LINKED_LIST_IMPLEMENTATION_INCLUDED_