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
#ifndef __FUNCTIONS_STRING_HEADER_INCLUDED_
#define __FUNCTIONS_STRING_HEADER_INCLUDED_

#define mLargeToSmall(c) (((c) >= 'A'&& (c) <= 'Z') ? ((c) - 'A'+'a') : (c))

void fLargeToSmall(char&);

bool myIsAlpha(char);

void skipSpaceInEnd(const char*, size_t&);

void skipToAlpha(char*&);

void skipWhiteSpace(char*&);

#endif //__FUNCTIONS_STRING_HEADER_INCLUDED_