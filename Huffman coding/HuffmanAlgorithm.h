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

#ifndef __HUFFMAN__ALGORITHM_HEADER_INCLUDED_
#define __HUFFMAN__ALGORITHM_HEADER_INCLUDED_

#include "HuffmanCoding.hpp"
#include "HuffmanDecoding.hpp"

namespace fmi{
namespace sdp {
namespace compression {

class HuffmanAlgorithm
	:public huffman::HuffmanCoding, public huffman::HuffmanDecoding {

public:
	HuffmanAlgorithm() = default;
	~HuffmanAlgorithm() = default;

private:
	HuffmanAlgorithm(const HuffmanAlgorithm&) = delete;
	void operator=(const HuffmanAlgorithm&) = delete;
};


}//namespace compression
}//namespace sdp
}//namespace fmi

#endif //__HUFFMAN__ALGORITHM_HEADER_INCLUDED_