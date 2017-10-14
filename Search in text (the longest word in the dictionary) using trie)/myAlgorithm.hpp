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
#ifndef __ALGORITHM_HEADER_INCLUDED_
#define __ALGORITHM_HEADER_INCLUDED_

#include <algorithm>
#include <cassert>

#include "mySwap.hpp"
#include "mySort.hpp"
#include "myQuickSort.hpp"

long long stepen(long long, long long, long long);

long double quicklyPow(long double, unsigned);

unsigned long long binom(unsigned, unsigned);

template<class DataType>
	inline size_t binarySearch(DataType *arr, size_t size, const DataType &value,
					bool(*p)(const DataType &first, const DataType &second)) {
		size_t left = 0, right = size - 1, middle;

		while (left <= right) {
			middle = left + (right - left) / 2;

			if (arr[middle] == value)				return middle;
			else if (p(arr[middle],value)==false)   left = middle + 1;
			else									right =middle - 1;
		}

		return size;
}

template<class First,class Second>
struct Pair{
	First first;
	Second second;

	Pair();
	Pair(const First &left, const Second &right) {
		first = left;
		second = right;
	}
};

#endif //__ALGORITHM_HEADER_INCLUDED_