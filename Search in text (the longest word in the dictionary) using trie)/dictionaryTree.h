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
#ifndef __DICTIONARY_TREE_HEDARED_INCLUDED_
#define __DICTIONARY_TREE_HEDARED_INCLUDED_

#include "linkedList.h"

struct Word {
	const char *beg;
	const char *end;
	bool endOfPhase;

	Word(const char *beg = nullptr, const char *end = nullptr, bool endOfPhare = true)
		: beg(beg)
		, end(end)
		, endOfPhase(endOfPhase) {}
};

class DictionaryTree {
public:
	DictionaryTree(long=0);
	~DictionaryTree();

	size_t add(const char*,size_t,long);
	long search(const char*,size_t) const;
	bool search(fmi::sdp::LinkedList<Word>&,long&) const;

	void setCoefficient(long);
	long getCoefficient() const;

	///*bool getFlagEndWord() const;
	//DictionaryTree* operator[](size_t) const;*/

	static size_t getAlphabetLen();

private:
	DictionaryTree **nextCharacter;
	long coefficient;
	bool endWord;

	//'z'+1;//unites words in the phrase
	static const char alphabetLen = 27;//26 lowercase + 1 char

	DictionaryTree(const DictionaryTree&) = delete;
	void operator=(const DictionaryTree&) = delete;
};

#endif //__DICTIONARY_TREE_HEDARED_INCLUDED_