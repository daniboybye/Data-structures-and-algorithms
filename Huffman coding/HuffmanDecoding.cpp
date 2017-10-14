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

#include "HuffmanDecoding.hpp"
#include "stack.hpp"
#include "pair.hpp"

using namespace fmi::sdp::huffman;
using namespace fmi::sdp::structures;

HuffmanDecoding::HuffmanDecoding()
: HuffmanTreeBasic()
, bitBuff(BUFF_SIZE << 4)
, buff()
{}

void HuffmanDecoding::uncompress(const char *codingFileName, const char *folderToUncompress, byte flag) {

	assert(codingFileName);
	assert(flag);

	{
		boost::filesystem::path fileOrDirectory(codingFileName);

		if (boost::filesystem::exists(fileOrDirectory) == false) {
			std::cerr << codingFileName << " is not exists" << std::endl;
			return;
		}
	}

	this->archiveFile.open(codingFileName, std::ios::binary);

	if ((this->archiveFile.good() && this->archiveFile.is_open()) == false) {
		this->archiveFile.close();
		std::cerr << codingFileName << " is not open!" << std::endl;
		return;
	}

	this->root = this->readFileAndDirectoryNames();

	if (flag & BitFlag::list) {
		this->root->print(std::cout);

		if ((flag&(~BitFlag::list)) == 0) {

			this->archiveFile.close();
			this->buff.size = 0;
			this->bitBuff.clear();
			delete this->root;
			return;
		}
	}


	if (flag & BitFlag::search) {

		this->search(folderToUncompress);

		if ((flag&(~BitFlag::search & ~BitFlag::list)) == 0) {
			this->archiveFile.close();
			this->buff.size = 0;
			delete this->root;
			return;
		}
	}

	std::string pathStr(folderToUncompress);
	pathStr.pop_back();

	{
		boost::filesystem::path currPath(pathStr);

		if ((boost::filesystem::exists(currPath) || boost::filesystem::create_directories(currPath)) == false) {
			std::cerr << currPath.filename().string() << " can't be created!" << std::endl;
			return;
		}
	}

	pathStr += '\\';
	pathStr += this->root->getName();

	if (this->root->isDirectory())
		this->restoreDirectories(pathStr);
	else {
		this->decodingFile(pathStr);
		std::cout << this->root->getName() << " is unarchived" << std::endl << std::endl;
	}


	assert(bitBuff.getSize() < 8);
	this->bitBuff.clear();

	delete this->root;

	this->archiveFile.close();
}

FilesystemInterface* HuffmanDecoding::readFileAndDirectoryNames() {

	Directory *directory;
	{
		uint16_t len = this->popOfBitBuff<uint16_t>();

		for (uint16_t pos = 0; pos < len; ++pos)
			this->buff.str[pos] = this->popOfBitBuff<char>();


		this->buff.str[len] = '\0';

		this->resizeBuff();

		if (this->bitBuff.dequeue()==HuffmanTreeBasic::DIRECTORY)
			directory = new Directory(this->buff.str);
		else
			return new File(this->buff.str);
	}

	uint32_t fileInDirectory = this->popOfBitBuff<uint32_t>();

	while (fileInDirectory--)
		directory->add(this->readFileAndDirectoryNames());
	
	return directory;
}

void HuffmanDecoding::restoreDirectories(std::string pathStr) {
	Pair<Directory*, int> put(dynamic_cast<Directory*>(this->root), 0);

	Stack< Pair<Directory*, int> > stack;

	stack.push(put);

	size_t size;
	long pos;


	this->createDirectory(pathStr);

	while (stack.getSize()) {

		for (pos = stack.front().second; pos < (long)stack.front().first->getSize(); ++pos) {

			pathStr += '\\';
			pathStr += stack.front().first->operator[](pos)->getName();

			if (stack.front().first->operator[](pos)->isDirectory()) {

				stack.front().second = pos + 1;

				put.first = dynamic_cast<Directory*>(stack.front().first->operator[](pos));
				stack.push(put);

				this->createDirectory(pathStr);

				pos = -1;

				continue;

			}
			else {
				this->decodingFile(pathStr);

				std::cout << stack.front().first->operator[](pos)->getName() << " is unarchived" << std::endl << std::endl;

				size = pathStr.size() - 1 - stack.front().first->operator[](pos)->getName().size();

				pathStr.resize(size);
			}
		}

		size = pathStr.size() - 1 - stack.pop().first->getName().size();

		pathStr.resize(size);

	}
}

void HuffmanDecoding::decodingFile(std::string &pathStr) {

	this->originalFile.open(pathStr,std::ios::binary);

	if ((this->originalFile.good() && this->originalFile.is_open()) == false) {
		this->originalFile.close();
		delete this->root;

		pathStr += " is not open!";

		throw std::ofstream::failure(pathStr);
	}


	size_t count = this->popOfBitBuff<byte>();
		
	if (count == 0) {
		this->specialCase();
		originalFile.close();
		return;
	}
			
	++count;

	std::vector<HuffmanTreeBasic::Note> alphabet;

	this->readAlphabet(alphabet,count);

	this->HuffmanTreeBasic::createTree(alphabet);

	uint64_t fileSize = 0;

	for (auto i = alphabet.cbegin(); i != alphabet.cend(); ++i)
		fileSize += i->value;
	

	while (fileSize--) {

		
		this->buff.str[this->buff.size++] = this->crawTheTree();

		if (BUFF_SIZE > this->buff.capacity - this->buff.size){

			if (this->buff.resize() == false) {

				this->originalFile.write(this->buff.str, this->buff.size);
				buff.size = 0;
			}

		}
	}

	

	this->originalFile.write(this->buff.str, this->buff.size);

	this->buff.size = 0;


#ifdef _DEBUG
	std::streampos pos = this->originalFile.tellp();
	this->originalFile.seekp(0, std::ios::end);
	assert(pos == this->originalFile.tellp());
#endif //_DEBUG
	
	
	this->HuffmanTreeBasic::clean();

	this->originalFile.close();
}

void HuffmanDecoding::createDirectory(std::string &pathStr) {

	boost::filesystem::path currPath(pathStr);

	if (boost::filesystem::create_directory(currPath) == false) {
		delete this->root;
		this->archiveFile.close();

		pathStr += " can't be created!";

		throw pathStr;
	}
}


void HuffmanDecoding::readAlphabet(std::vector<HuffmanTreeBasic::Note> &alphabet,size_t count) {
	char symbol;
	uint32_t value;

	for (size_t i = 0; i < count; ++i) {

		symbol = this->popOfBitBuff<char>();
		value = this->popOfBitBuff<uint32_t>();

		alphabet.push_back(Note(value,symbol));
	}
}

void HuffmanDecoding::specialCase() {

	char symbol = this->popOfBitBuff<char>();
	uint32_t value = this->popOfBitBuff<uint32_t>();

	if (value) {
		memset(this->buff.str, symbol,value);
		this->originalFile.write(this->buff.str, this->buff.size);
		this->buff.size=0;
	}
}

char HuffmanDecoding::crawTheTree() {
	
	this->resizeBuff();

	size_t curr = 0;

	while (this->tree.vertices[curr].leaf == 0) {

		if (this->bitBuff.dequeue())
			curr = this->tree.vertices[curr].childLeft;
		else 
			curr = this->tree.vertices[curr].childLeft + 1;
	
	}

	return this->tree.vertices[curr].symbol;
}

void HuffmanDecoding::resizeBuff() {
	if (this->bitBuff.getSize() < BUFF_SIZE && this->archiveFile.good()) {
		
		this->archiveFile.read(this->buff.str + this->buff.size, BUFF_SIZE);

		this->bitBuff.enqueue(this->buff.str + this->buff.size, this->archiveFile.gcount());
	}
}

static void KMPcreateTable(std::vector<size_t> &table, const char *str) {
	table.push_back(0);

	for (size_t pos = 1; str[pos]; ++pos) {

		table.push_back(table.back());

		while (table[pos] && str[table[pos]] != str[pos])
			table[pos] = table[table[pos] - 1];

		if (str[table[pos]] == str[pos])
			++table[pos];
	}
}

void HuffmanDecoding::search(const char *str) {

	std::vector<size_t> table;
	KMPcreateTable(table, str);

	if (this->root->isDirectory()) {
		Pair<Directory*, int> put(dynamic_cast<Directory*>(this->root), 0);

		Stack< Pair<Directory*, int> > stack;

		stack.push(put);

		size_t size;
		long pos;

		while (stack.getSize()) {

			for (pos = stack.front().second; pos < (long)stack.front().first->getSize(); ++pos) {


				if (stack.front().first->operator[](pos)->isDirectory()) {

					stack.front().second = pos + 1;

					put.first = dynamic_cast<Directory*>(stack.front().first->operator[](pos));
					stack.push(put);

					pos = -1;

					continue;
				}
				else {
					std::cout << "searching in " << stack.front().first->operator[](pos)->getName() << std::endl;
					std::cout << "matches: " << this->searchInArchiveFile(table,str) << std::endl << std::endl;
					
				}
			}

			stack.pop();
		}
	}
	else {
		std::cout << "searching in " << this->root->getName() << std::endl;
		std::cout << "matches " << this->searchInArchiveFile(table,str) << std::endl << std::endl;
	}
}

uint32_t HuffmanDecoding::searchInArchiveFile(const std::vector<size_t> &table, const char *str) {
	uint32_t value = 0,match=0;

	uint32_t count = this->popOfBitBuff<byte>();

	if (count == 0) {
		char symbol = this->popOfBitBuff<char>();
		count = this->popOfBitBuff<uint32_t>();

		if (value >= table.size() && table.back() == table.size() - 1 && symbol == str[0])
			return value / table.size();

		
		return 0;
	}

	++count;

	std::vector<HuffmanTreeBasic::Note> alphabet;

	this->readAlphabet(alphabet, count);

	this->HuffmanTreeBasic::createTree(alphabet);

	uint64_t fileSize = 0;

	for (auto i = alphabet.cbegin(); i != alphabet.cend(); ++i)
		fileSize += i->value;


	for (uint64_t i = 0; i < fileSize;++i) {

		while (i<fileSize && match < table.size() && str[match] == this->crawTheTree()) {

			++match;

			++i;
		}
		
		if (match == table.size()) {
			++value;
			match = 0;
			--i;
			continue;
		}

		if (match)
			--match;

		match = table[match];
	}

	return value;
}

HuffmanDecoding::Buff::Buff()
: size(0)
, capacity(BUFF_SIZE << 1) {

	this->str = (char*)malloc(BUFF_SIZE << 1);

	if (this->str == nullptr)
		throw std::bad_alloc();
}

HuffmanDecoding::Buff::~Buff() {
	free(this->str);
}

bool HuffmanDecoding::Buff::resize() {
	if (this->capacity > 16e7)
		return false;

	char *p = (char*)realloc(this->str, this->capacity << 1);
	if (p) {
		this->str = p;
		this->capacity <<= 1;
		return true;
	}
	return false;
}