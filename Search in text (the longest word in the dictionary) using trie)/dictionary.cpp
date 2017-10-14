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
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

#include "dictionary.h"
#include "linkedList.h"
#include "stringFunctions.h"

#define BUFF_SIZE 4096

static long extractNumber(const char *str, size_t &len) {
	long number = 0, step = 1;

	skipSpaceInEnd(str, len);

	while (str[len - 1] >= '0'&&str[len - 1] <= '9') {
		--len;
		number += step*(str[len] - '0');
		step *= 10;
	}

	if (str[len - 1] == '-'){
		--len;
		number *= (-1);
	}
	else if (str[len - 1] == '+') {
		--len;
	}

	skipSpaceInEnd(str, len);

	return number;
}


Dictionary::Dictionary()
:maxWordsInPhase(0) {
	this->firstCharacter = new DictionaryTree*[DictionaryTree::getAlphabetLen()]();
}

Dictionary::~Dictionary() {
	for (size_t i = 0; i < DictionaryTree::getAlphabetLen(); ++i) delete this->firstCharacter[i];

	delete[] this->firstCharacter;
}



void Dictionary::addWordOrPhase(const char *str, size_t len, long coef) {
	if (len == 0 || str == nullptr) return;

	char pos = mLargeToSmall(str[0]) - 'a';

	if (this->firstCharacter[pos] == nullptr) {
		this->firstCharacter[pos] = new DictionaryTree;
	}

	size_t size = 1;

	if (len == 1){
		this->firstCharacter[pos]->setCoefficient(coef);
	}
	else{
		size += this->firstCharacter[pos]->add(str + 1, len - 1, coef);
	}

	if (this->maxWordsInPhase < size) this->maxWordsInPhase = size;
}

long Dictionary::search(const char *str, size_t len) const {
	if (len == 0 || str == nullptr) return 0;

	assert(myIsAlpha(str[0]));

	char pos = mLargeToSmall(str[0]) - 'a';

	if (this->firstCharacter[pos] == nullptr) return 0;

	if (len == 1) return this->firstCharacter[pos]->getCoefficient();

	return this->firstCharacter[pos]->search(str + 1, len - 1);
}

//bool deleteWordOrPhase(const char*,size_t);


void Dictionary::addFileWithWords(const char *fileName) {
	std::ifstream file(fileName);
	if ((file.good() && file.is_open()) == false) {
		file.close();

		std::cout << fileName << " is not open!" << std::endl;
		return;
	}

	size_t len;
	long coef;
	char buff[BUFF_SIZE];

	do{
		file.getline(buff, BUFF_SIZE);

		len = strlen(buff);

		coef = extractNumber(buff, len);

		this->addWordOrPhase(buff, len, coef);

	} while (file.peek() != std::char_traits<std::ifstream>::eof() && file.good());

	file.close();
}

void Dictionary::calculateFileCoefficient(const char *fileName) const {

	std::ifstream file(fileName,std::ios::binary);

	if ((file.good() && file.is_open()) == false) {
		file.close();
		std::cout << fileName << " is not open!" << std::endl;
		return;
	}

	char buff[BUFF_SIZE];
	buff[BUFF_SIZE - 1]='\0';

	long long value = 0;
	unsigned long long countWords = 0;
	const char *beg;
	size_t size;

	fmi::sdp::LinkedList<Word> list;

	do{
		size = 0;

		if (list.getSize()) {
			beg = list.front().beg;
			size = beg - &buff[0];

			fmi::sdp::LinkedList<Word>::Iterator it = list.getIterator();

			while (it) {
				it->beg -= size;

				if (it->end) it->end -= size;

				++it;
			}

			size = &buff[BUFF_SIZE - 1] - beg;
			memcpy(&buff[0], beg, size);
		}

		memset(&buff[size], 0, BUFF_SIZE - 1 - size);
		file.read(&buff[size], BUFF_SIZE - 1 - size);
		translateBuffInList(buff + size, list);

		if (file.peek() == std::char_traits<std::ifstream>::eof() || file.eof()) {

			file.close();

			countWords += list.getSize();

			if (list.getSize()) {
				list.back().endOfPhase = true;

				if (list.back().end == nullptr) {
					list.back().end = &buff[strlen(buff)];
				}
			}

			while (list.getSize()) {
				value += this->searchRecursive(list);
			}
				
			break;
		}
		else {

			countWords += list.getSize();

			while (list.getSize() > this->maxWordsInPhase) {
				value += this->searchRecursive(list);
			}

			countWords -= list.getSize();
		}
	} while (file.good());


	std::cout << fileName << " " << std::fixed << std::setprecision(2) << (double)value / countWords << std::endl;
}

void Dictionary::translateBuffInList(char *str, fmi::sdp::LinkedList<Dictionary::Word> &list) const {
	Word newWord;

	if (list.getSize() && list.back().end == nullptr) {
		while (myIsAlpha(*str)) ++str;
		list.back().end = str;
	}

	if (list.getSize()) {
		skipWhiteSpace(str);
		if (myIsAlpha(*str)==false) {
			list.back().endOfPhase = true;
		}
	}

	skipToAlpha(str);
	while (*str) {
		newWord.beg = str;
		newWord.end = nullptr;

		while (myIsAlpha(*str)) {
			*str = mLargeToSmall(*str);
			++str;
		}


		if (*str == '\0'){
			newWord.endOfPhase = false;
			list.pushBack(newWord);
			return;
		}
		else {
			newWord.end = str;
		}

		skipWhiteSpace(str);

		if (*str=='\0' || myIsAlpha(*str)) {
			newWord.endOfPhase = false;
		}
		else {
			newWord.endOfPhase = true;
			skipToAlpha(str);
		}

		list.pushBack(newWord);
	}
}

long Dictionary::searchRecursive(fmi::sdp::LinkedList<Dictionary::Word> &list) const {
	assert(list.front().end > list.front().beg);
	assert(myIsAlpha(list.front().beg[0]));

	char pos = list.front().beg[0] - 'a';
	long value;

	if (this->firstCharacter[pos]) {

		++list.front().beg;

		if (this->firstCharacter[pos]->search(list, value)) {
			return value;
		}
		else {
			return 0;
		}
	}

	list.popFront();
	return 0;
}