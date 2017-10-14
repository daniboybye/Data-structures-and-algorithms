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
#include "HuffmanTreeBasic.h"

using namespace fmi::sdp::huffman;

void HuffmanTreeBasic::createTable() {

	size_t child, parent;
	size_t pos = this->tree.size;

	while (pos--) {
		if (this->tree.vertices[pos].leaf) {

			child = pos;

			do {
				parent = this->tree.vertices[child].parent;

				if (this->tree.vertices[parent].childLeft == child)
					this->code[(byte)this->tree.vertices[pos].symbol].push_back(1);
				else
					this->code[(byte)this->tree.vertices[pos].symbol].push_back(0);

				child = parent;

			} while (child);
		}
	}
}

void HuffmanTreeBasic::contactWithChildren(size_t pos) {
	if (this->tree.vertices[pos].childLeft) {
		this->tree.vertices[this->tree.vertices[pos].childLeft].parent = pos;
		this->tree.vertices[this->tree.vertices[pos].childLeft + 1].parent = pos;
	}
}

void HuffmanTreeBasic::clean() {
	size_t i = UCHAR_MAX + 1;
	while (i--)
		this->code[i].resize(0);
}

HuffmanTreeBasic::Note::Note()
: leaf(1)
, childLeft(0)
, parent(300)
, value(0)
{}

HuffmanTreeBasic::Note::Note(uint32_t value,char symbol)
: leaf(1)
, childLeft(0)
, parent(300)
, value(value)
, symbol(symbol)
{}

HuffmanTreeBasic::Note::Note(const Note &left, const Note &right, unsigned short pos)
: leaf(0)
, childLeft(pos)
, value(left.value + right.value)
, parent(300)
{}

void HuffmanTreeBasic::createTree(std::vector<Note> &alphabet) {

	structures::Heap<Note> heap(alphabet);

	size_t pos = (heap.getSize() << 1) - 1;

	this->tree.size = pos;

	while (heap.getSize()>1) {

		this->tree.vertices[--pos] = heap.pop();
		this->contactWithChildren(pos);

		this->tree.vertices[--pos] = heap.pop();
		this->contactWithChildren(pos);

		heap.push(Note(this->tree.vertices[pos], this->tree.vertices[pos + 1], pos));
	}

	this->tree.vertices[0] = heap.pop();
	this->contactWithChildren(0);

	this->createTable();
}