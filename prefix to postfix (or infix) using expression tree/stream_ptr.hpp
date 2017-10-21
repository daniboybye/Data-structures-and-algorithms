/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Daniel Urumov
* @idnumber 45135
* @task 1
* @compiler VC
*
*/
#pragma once

#ifndef __STREAM_PTR_UNCLUDED_
#define __STREAM_PTR_UNCLUDED_

#include <memory>
#include <fstream>

template <typename stream>
struct Deleter_stream
{
	void operator()(stream * obj) const
	{
		if(obj->is_open())
			obj->close();

		delete obj;
	}
};

template struct Deleter_stream<std::ofstream>;
template struct Deleter_stream<std::ifstream>;

typedef std::unique_ptr<std::ofstream, Deleter_stream<std::ofstream>> unique_ofstream;
typedef std::unique_ptr<std::ifstream, Deleter_stream<std::ifstream>> unique_ifstream;

#endif //__STREAM_PTR_UNCLUDED_