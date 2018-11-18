#include "KongaSet.h"
#include <iostream>

KongaSet::KongaSet() {
	set.push_back(new Konga);
}

KongaSet::~KongaSet() {
	for (int i = 0; i < set.getSize(); ++i) {
		delete set[i];
	}
}

bool KongaSet::append(const Student& student, int index) {
	if (student.tolerate(set[index]->back())) {
		set[index]->pushBack(student);
		return true;
	}
	return false;
}

void KongaSet::removeFirst(int index) {
	set[index]->popFront();
	remove_if_empty(index);
}

void KongaSet::removeLast(int index) {
	set[index]->popBack();
	remove_if_empty(index);
}

bool KongaSet::merge(int firstIndex, int secondIndex) {
	if (set[secondIndex]->front().tolerate(set[firstIndex]->back())) {
		set[firstIndex]->append(*set[secondIndex]);
		remove_if_empty(secondIndex);
		return true;
	}
	return false;
}

bool studentNameIs(const Student& st, const std::string& name) {
	return st.name == name;
}

void KongaSet::remove(const std::string& name, int index) {
	Konga* newKonga = set[index]->split_if(name, studentNameIs);

	after_split(newKonga, index);
}

void KongaSet::remove_with_lambda(const std::string& name, int index) {
	Konga* newKonga = set[index]->split_if([name](const Student& st) {
		return st.name == name;
	});
	after_split(newKonga, index);
}

void KongaSet::after_split(Konga* newKonga, int index) {
	if (newKonga) {
		newKonga->popFront();
		set.push_back(newKonga);
		remove_if_empty(set.getSize() - 1);
	}

	remove_if_empty(index);
}

void KongaSet::print() const {
	for (int i = 0; i < set.getSize(); ++i) {
		std::cout << "Line" << i << ": ";

		set[i]->for_each(printStudent);

		std::cout << std::endl;
	}
}

void KongaSet::remove_if_empty(int index) {
	if (set[index]->empty()) {
		std::swap(set[index], set.back());
		delete set.back();
		set.popBack();
	}
}

bool KongaSet::empty() const {
	return set.getSize() == 0;
}

int KongaSet::size() const {
	return set.getSize();
}