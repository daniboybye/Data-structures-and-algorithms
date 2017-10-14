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

#ifndef __HUFFMAN_DECODING_HEADER_INCLUDED_
#define __HUFFMAN_DECODING_HEADER_INCLUDED_

#include "HuffmanTreeBasic.h"
#include "filesystemInterface.h"

namespace fmi{
namespace sdp{
namespace huffman {

class HuffmanDecoding
: virtual public HuffmanTreeBasic {

public:
	HuffmanDecoding();

	~HuffmanDecoding() = default;

	void list(const char *codingFileName) {
		this->uncompress(codingFileName, nullptr,BitFlag::list);
	}

	void unpack(const char *codingFileName, const char *folderToUncompress) {
		assert(folderToUncompress);
		this->uncompress(codingFileName, folderToUncompress,BitFlag::unpack);
	}

	void search(const char *codingFileName, const char *str) {
		assert(str);
		this->uncompress(codingFileName, str, BitFlag::search);
	}

private:
	bitStructures::BitQueue bitBuff;
	std::ofstream originalFile;
	std::ifstream  archiveFile;

	struct Buff{
		char *str;
		size_t size, capacity;

		Buff();

		~Buff();

		bool resize();
	}buff;

	structures::FilesystemInterface *root;

	structures::FilesystemInterface *readFileAndDirectoryNames();

	struct BitFlag {
		static const byte list = 1;
		static const byte search = 2;
		static const byte unpack = 4;
	};

	void uncompress(const char *codingFileName, const char *folderToUncompress,byte bitFlag);

	void createDirectory(std::string &pathStr);

	void restoreDirectories(std::string pathStr);

	void decodingFile(std::string &pathStr);

	void resizeBuff();

	void readAlphabet(std::vector<HuffmanTreeBasic::Note> &alphabet, size_t count);

	void specialCase();

	char crawTheTree();

	void search(const char *str);

	uint32_t searchInArchiveFile(const std::vector<size_t> &table, const char *str);

	template<typename DataType>
	DataType popOfBitBuff() {
		DataType value = 0;

		uint32_t mask = sizeof(DataType);
		mask <<= 3;

		if (mask >= this->bitBuff.getSize()) 
			this->resizeBuff();
		

		while (mask--)
		if (this->bitBuff.dequeue())
			value |= (1 << mask);

		return value;
	}

	HuffmanDecoding(const HuffmanDecoding&) = delete;

	void operator=(const HuffmanDecoding&) = delete;
};


}//namespace huffman
}//namespace sdp
}//namespace fmi

#endif //__HUFFMAN_DECODING_HEADER_INCLUDED_