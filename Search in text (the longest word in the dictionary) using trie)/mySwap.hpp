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
#ifndef __SWAP_HEADER_INCLUDED_
#define __SWAP_HEADER_INCLUDED_

template<class DataType>
inline void swapif(DataType &left, DataType &right, bool(*p)(const DataType &first, const DataType &second)) {
	if (p(left, right)) {
		std::swap(left, right);
	}
}

template<class DataType>
inline void swapif(DataType &left, DataType &right) {
	if (left< right) {
		std::swap(left, right);
	}
}


#endif //__SWAP_HEADER_INCLUDED_