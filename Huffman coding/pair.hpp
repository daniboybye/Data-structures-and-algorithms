/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Daniel Urumov
* @idnumber 45135
* @task 0
* @compiler VC
*
*/
#pragma once

#ifndef __PAIR_HEADER_UNCLUDED_
#define __PAIR_HEADER_UNCLUDED_

namespace fmi {
namespace sdp{
namespace structures {

template<typename First, typename Second>
struct Pair{
	First first;
	Second second;

	Pair() 
	{}

	Pair(const First &left, const Second &right)
	:first(left)
	,second(right)
	{}
};

}//namespace structures 
}//namespace sdp
}//namespace fmi



#endif //__PAIR_HEADER_UNCLUDED_