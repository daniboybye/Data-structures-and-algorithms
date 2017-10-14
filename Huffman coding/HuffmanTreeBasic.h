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

#ifndef __HUFFMAN_TREE_BASIC_HEADER_INCLUDED_
#define __HUFFMAN_TREE_BASIC_HEADER_INCLUDED_

#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>

#include "bitVector.hpp"
#include "bitQueue.h"
#include "heap.hpp"


//(1) read uint16_t(len) + ?(name) + bit
//(2) if(bit==DIRECTORY)  uint32_t(how files are)  => (1)
//(3) else byte(count-1 symbols) + char(symbol) + uint32_t(value) + coding



namespace fmi{
namespace sdp{
namespace huffman {


typedef unsigned char byte;
#define BUFF_SIZE 4096


class HuffmanTreeBasic {
public:
	~HuffmanTreeBasic()=default;

protected:
	HuffmanTreeBasic()=default;

	struct Note {
		uint32_t value;
		uint16_t leaf : 1;
		uint16_t parent : 15;
		uint16_t childLeft;
		char symbol;

		Note();

		Note(uint32_t value,char symbol);

		Note(const Note &left, const Note &right, uint16_t pos);

		friend bool operator<(const Note &left, const Note &right) {
			return left.value < right.value;
		}
	};

	struct Tree {
		Note vertices[511]; //2*256-1
		size_t size;
	};

	static const bool FILE = false;
	static const bool DIRECTORY = true;

	std::vector<byte> code[UCHAR_MAX + 1];

	Tree tree;

	void createTree(std::vector<Note> &alphabet);

	void clean();

private:

	void createTable();

	void contactWithChildren(size_t pos);

	HuffmanTreeBasic(const HuffmanTreeBasic&) = delete;

	HuffmanTreeBasic& operator=(const HuffmanTreeBasic&) = delete;
};


}//namespace huffman
}//namespace sdp
}//namespace fmi


#endif //_HUFFMAN_TREE_BASIC_HEADER_INCLUDED_