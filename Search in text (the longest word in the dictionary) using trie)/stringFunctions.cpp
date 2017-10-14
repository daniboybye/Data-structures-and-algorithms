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
#include <cassert>

#include "stringFunctions.h"

void fLargeToSmall(char &c) {
	c = mLargeToSmall(c);
}

bool myIsAlpha(char c) {
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z')) {
		return true;
	}
	return false;
}

void skipSpaceInEnd(const char *str, size_t &len) {
	while (len && (str[len - 1] == ' ' || str[len - 1] == '\t'
		|| str[len - 1] == '\n' || str[len - 1] == '\r'))
		--len;
}

void skipToAlpha(char *&str) {
	assert(str);

	while (*str) {
		skipWhiteSpace(str);

		if (myIsAlpha(*str))
			return;
		else
			++str;
	}
}

void skipWhiteSpace(char *&str) {
	assert(str);

	while (*str) {
		if (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n')
			++str;
		else
			return;
	}
}