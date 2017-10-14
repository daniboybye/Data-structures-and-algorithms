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
#include "dictionary.h"
#include "stringFunctions.h"
#include <fstream>
#include <cassert>

Dictionary::DictionaryTree::DictionaryTree(long coefficient)
: coefficient(coefficient)
, endWord(false) {
	this->nextCharacter = new DictionaryTree*[DictionaryTree::alphabetLen]();
}

Dictionary::DictionaryTree::~DictionaryTree() {
	for (size_t i = 0; i < DictionaryTree::alphabetLen; ++i) delete this->nextCharacter[i];

	delete[] this->nextCharacter;
}

size_t Dictionary::DictionaryTree::add(const char *str, size_t len, long coef) {
	char pos;
	if (myIsAlpha(str[0])) {
		pos = mLargeToSmall(str[0]) - 'a';
	}
	else pos = DictionaryTree::alphabetLen-1;

	if (this->nextCharacter[pos] == nullptr) {
		this->nextCharacter[pos] = new DictionaryTree;
	}

	if (len == 1) {
		assert(pos < DictionaryTree::alphabetLen - 1);
		this->nextCharacter[pos]->setCoefficient(coef);
		return 0;
	}
	else if (pos == DictionaryTree::alphabetLen - 1) {
		return 1 + this->nextCharacter[pos]->add(str + 1, len - 1, coef);
	}
	return this->nextCharacter[pos]->add(str + 1, len - 1, coef);
}

long Dictionary::DictionaryTree::search(const char *str, size_t len) const {

	if (str[0] == ' ' || str[0] == '\t' || str[0] == '\n'){

		if (this->nextCharacter[DictionaryTree::alphabetLen - 1]){

			long value= this->nextCharacter[DictionaryTree::alphabetLen - 1]->search(str + 1, len - 1);

			if (value)
				return value;
			else
				return this->coefficient;
		}
		
		return 0;
	}

	assert(myIsAlpha(str[0]));

	char pos = mLargeToSmall(str[0]) - 'a';

	if (this->nextCharacter[pos] == nullptr) return 0;

	if (len == 1) return this->nextCharacter[pos]->getCoefficient();

	return this->nextCharacter[pos]->search(str + 1, len - 1);
}

bool Dictionary::DictionaryTree::search(fmi::sdp::LinkedList<Word> &list,long &value) const {
	if (list.front().end == list.front().beg) {

		if (list.popFront().endOfPhase==false && this->nextCharacter[DictionaryTree::alphabetLen - 1] != nullptr){
		
			assert(list.getSize());

			Word word(list.front());//save original

			if (this->nextCharacter[DictionaryTree::alphabetLen - 1]->search(list, value)) {
				return true;;
			}
			else{
				list.pushFront(word);
			}
		}
		
		if (this->endWord) {
			value = this->coefficient;
			return true;
		}
		else {
			return false;
		}
	}

	assert(myIsAlpha(list.front().beg[0]));

	char pos = list.front().beg[0] - 'a';

	if (this->nextCharacter[pos] == nullptr) {
		list.popFront();
		return false;
	}

	++list.front().beg;
	return this->nextCharacter[pos]->search(list,value);
}

size_t Dictionary::DictionaryTree::getAlphabetLen() {
	return DictionaryTree::alphabetLen-1;
}

void Dictionary::DictionaryTree::setCoefficient(long coefficient) {
	this->endWord = true;
	this->coefficient = coefficient;
}

long Dictionary::DictionaryTree::getCoefficient() const {
	return this->coefficient;
}

//bool Dictionary::DictionaryTree::getFlagEndWord() const {
//	return this->endWord;
//}
//
//Dictionary::DictionaryTree* Dictionary::DictionaryTree::operator[](size_t index) const {
//	if (index >= Dictionary::DictionaryTree::alphabetLen) {
//		return nullptr;
//	}
//
//	return this->nextCharacter[index];
//}