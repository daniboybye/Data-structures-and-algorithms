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
#include "string.h"
#include <cassert>

namespace fmi{
namespace sdp {

String::String()
:size(1){
	this->str = new('\0') char[this->size];
}

String::String(const char *str, size_t size){
	assert(str);

	this->size = size + 1;
	this->str = new char[this->size];
	this->str[this->size - 1] = '\0';

	for (size_t i = 0; i < size; ++i) this->str[i]=str[i];
}

String::String(const String &other)
	:size(other.size) {
	this->str = new char[this->size];

	for (size_t i = 0; i < this->size; ++i) this->str[i] = other.str[i];
}

String& String::operator=(const String &other) {
	if (this != &other) {
		delete[] this->str;
		this->size = other.size;
		this->str = new char[this->size];
		for (size_t i = 0; i < this->size; ++i) this->str[i] = other.str[i];
	}
	return *this;
}

String::~String() {
	delete[] this->str;
}

String& String::operator+=(const String &other) {
	size_t newSize=this->size + other.size - 1;
	char *strcat = new char[newSize];
	strcat[newSize - 1] = '\0';

	size_t i = 0;
	for (; i < this->size - 1; ++i) strcat[i] = this->str[i];

	delete[] this->str;
	this->str = strcat;
	this->size = newSize;

	for (; i < this->size - 1; ++i) this->str[i] = other.str[i];

	return *this;
}

const char& String::operator[](size_t index) const {
	assert(index < this->size - 1);
	return this->str[index];
}
	 
char& String::operator[](size_t index) {
	assert(index < this->size - 1);
	return this->str[index];
}

size_t String::getSize() const {
	return this->size-1;
}

bool operator==(const String &first, const String &second) {
	if (first.getSize() != second.getSize()) return false;

	for (size_t i = 0; i < first.getSize();++i)
		if (first[i] != second[i]) return false;

	return true;
}

bool operator!=(const String &first, const String &second) {
	return !(first == second);
}

String operator+(const String &first, const String &second) {
	return String(first) += second;
}
	
std::ostream& operator<<(std::ostream &os, const String &str) {
	os << static_cast<const char*>(str);

	return os;
}

String::operator const char*() const {
	return this->str;
}

}
}