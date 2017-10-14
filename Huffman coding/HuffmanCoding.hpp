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

#ifndef __HUFFMAN_CODING_HEADER_INCLUDED_
#define __HUFFMAN_CODING_HEADER_INCLUDED_

#include "HuffmanTreeBasic.h"

namespace fmi{
namespace sdp{
namespace huffman {


class HuffmanCoding
: virtual public HuffmanTreeBasic {

public:
	HuffmanCoding();

	~HuffmanCoding() = default;

	void pack(const char *originalFileName, const char *codingFileName);

private:
	bitStructures::BitVector bitBuff;
	std::ofstream archiveFile;
	std::ifstream originalFile;

	void compressFile(const boost::filesystem::path &fileOrDirectory);

	void createArchive();

	void archive();

	void createAlphabet(std::vector<HuffmanTreeBasic::Note>&alphabet);

	void specialCase(std::vector<HuffmanTreeBasic::Note> &alphabet);

	void writeFileAndDirectoryNames(const boost::filesystem::path &fileOrDirectory) ;

	void writeName(const boost::filesystem::path &fileOrDirectory);

	void writeAlphabet(const std::vector<HuffmanTreeBasic::Note> &alphabet);

	void iterateDirectory(const boost::filesystem::path &directory);

	template<typename DataType>
	void pushInBitBuff(DataType data) {

		size_t mask = sizeof(data);
		mask <<= 3;

		while (mask--) {
			if (this->bitBuff.push_back(data&(1 << mask)) == false) {

				this->writeInFile(false);

				this->bitBuff.push_back(data&(1 << mask));
			}
		}
	}

	template<>
	void pushInBitBuff(bool data) {
		if (this->bitBuff.push_back(data) == false) {
			
			this->writeInFile(false);

			this->bitBuff.push_back(data);
		}
	}

	template<typename Iterator>
	void pushInBitBuff(Iterator &ref1, Iterator &ref2, size_t size) {
		if (this->bitBuff.append(ref1,ref2,size) == false) {

			this->writeInFile(false);

			this->bitBuff.append(ref1, ref2, size);
		}
	}

	void writeInFile(bool lastTime);

#if _DEBUG

	std::ofstream textFile;

#endif //_DEBUG

	HuffmanCoding(const HuffmanCoding&) = delete;

	void operator=(const HuffmanCoding&) = delete;
};

}//namespace huffman
}//namespace sdp
}//namespace fmi


#endif //__HUFFMAN_CODING_HEADER_INCLUDED_