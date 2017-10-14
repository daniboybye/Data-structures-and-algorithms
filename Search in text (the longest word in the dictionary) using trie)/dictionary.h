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
#ifndef __DICTIONARY_HEADER_INCLUDED_
#define __DICTIONARY_HEADER_INCLUDED_

#include "linkedList.h"

class Dictionary{
public:
	Dictionary();
	~Dictionary();

	void addFileWithWords(const char*);
	void calculateFileCoefficient(const char*) const;
	void addWordOrPhase(const char*, size_t, long);
	long search(const char*, size_t) const;
	//bool deleteWordOrPhase(const char*,size_t);

private:

#include "dictionaryTree.h"

	DictionaryTree **firstCharacter;
	size_t maxWordsInPhase;
	
	long searchRecursive(fmi::sdp::LinkedList<Dictionary::Word>&) const;
	//long searchIter(fmi::sdp::LinkedList<Dictionary::Word>&) const;
	void translateBuffInList(char*, fmi::sdp::LinkedList<Dictionary::Word>&) const;
	Dictionary(const Dictionary&) = delete;
	void operator=(const Dictionary&) = delete;
};

#endif //__DICTIONARY_HEADER_INCLUDED_