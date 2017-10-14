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

#include "HuffmanCoding.hpp"

using namespace fmi::sdp::huffman;
using namespace fmi::sdp::structures;

HuffmanCoding::HuffmanCoding()
: HuffmanTreeBasic()
, bitBuff()
{}

void HuffmanCoding::pack(const char *originalFileName, const char *codingFileName) {

	assert(originalFileName);
	assert(codingFileName);

	boost::filesystem::path fileOrDirectory(originalFileName);

	if (boost::filesystem::exists(fileOrDirectory) == false) {
		std::cerr << originalFileName << " is not exists" << std::endl;
		return;
	}

	this->archiveFile.open(codingFileName, std::ios::binary);

	if ((this->archiveFile.good() && this->archiveFile.is_open()) == false) {
		this->archiveFile.close();
		std::cerr << codingFileName << " is not open!" << std::endl;
		return;
	}

#if _DEBUG

	this->textFile.open("namesAreRecordedCorrectly.txt");

#endif //_DEBUG

	this->writeFileAndDirectoryNames(fileOrDirectory);

	this->compressFile(fileOrDirectory);
	
	this->writeInFile(true);

	this->archiveFile.close();

#if _DEBUG

	this->textFile.close();

#endif //_DEBUG	
}

void HuffmanCoding::writeFileAndDirectoryNames(const boost::filesystem::path &fileOrDirectory) {

	this->writeName(fileOrDirectory);

	if (boost::filesystem::is_directory(fileOrDirectory)) {
		this->pushInBitBuff(HuffmanTreeBasic::DIRECTORY);

		this->iterateDirectory(fileOrDirectory);
	}
	else {
		this->pushInBitBuff(HuffmanTreeBasic::FILE);
	}
}

void HuffmanCoding::iterateDirectory(const boost::filesystem::path &directory) {

	auto i = boost::filesystem::directory_iterator(directory);

	{
		uint32_t filesInDirectory = 0;

		for (; i != boost::filesystem::directory_iterator(); ++i, ++filesInDirectory);

		this->pushInBitBuff(filesInDirectory);
	}

	for (i = boost::filesystem::directory_iterator(directory);
		i != boost::filesystem::directory_iterator(); ++i) {

		this->writeFileAndDirectoryNames(i->path());
	}
}

void HuffmanCoding::writeName(const boost::filesystem::path &fileOrDirectory) {

	uint16_t len = fileOrDirectory.filename().string().size();

	this->pushInBitBuff(len);

	for (uint16_t pos = 0; pos < len; ++pos)
		this->pushInBitBuff(fileOrDirectory.filename().string()[pos]);

#if _DEBUG

	this->textFile << len << " " << fileOrDirectory.filename().string().c_str() << std::endl;

#endif //_DEBUG
}
void HuffmanCoding::compressFile(const boost::filesystem::path &fileOrDirectory) {

	if (boost::filesystem::is_directory(fileOrDirectory)) {

		std::cout << fileOrDirectory.filename().string() << " is in the process of archiving" << std::endl << std::endl;

		for (auto i = boost::filesystem::directory_iterator(fileOrDirectory); i != boost::filesystem::directory_iterator(); ++i)
			this->compressFile(i->path());

		std::cout << fileOrDirectory.filename().string() << " - directory has been archived" << std::endl << std::endl;

		return;
	}

	this->originalFile.open(fileOrDirectory.string(), std::ios::binary);

	if ((this->originalFile.good() && this->originalFile.is_open()) == false) {
		std::cerr << fileOrDirectory.string() << " in not open!" << std::endl;
		std::cerr << "The file won't be archived!" << std::endl;
		this->originalFile.close();
		return;
	}

	this->createArchive();

	this->originalFile.close();

	std::cout << fileOrDirectory.filename().string() << " has been archived" << std::endl << std::endl;;
}

void HuffmanCoding::createArchive() {

	std::vector<HuffmanTreeBasic::Note> alphabet;//vector helps to create a Heap for O(N)
	this->createAlphabet(alphabet);

	if (alphabet.size() > 1) {
		this->writeAlphabet(alphabet);
		this->HuffmanTreeBasic::createTree(alphabet);
		this->archive();

		this->HuffmanTreeBasic::clean();
	}
	else {
		this->specialCase(alphabet);
	}
}


void HuffmanCoding::createAlphabet(std::vector<HuffmanTreeBasic::Note> &alphabet) {

	uint32_t count[UCHAR_MAX + 1];
	memset(count, 0, sizeof(uint32_t)*(UCHAR_MAX + 1));

	byte buff[BUFF_SIZE];
	const byte *pStr;

	do {
		this->originalFile.read(reinterpret_cast<char*>(buff), BUFF_SIZE);

		pStr = buff;

		while (pStr != buff + this->originalFile.gcount())
			++count[*pStr++];

	} while (this->originalFile.good() &&
		this->originalFile.peek() != std::char_traits<std::ifstream>::eof());

	this->originalFile.clear();
	this->originalFile.seekg(0, std::ios::beg);



	for (size_t i = 0; i < UCHAR_MAX + 1; ++i) {
		if (count[i]) {
			alphabet.push_back(Note(count[i], char(i)));
		}
	}
}

void HuffmanCoding::writeAlphabet(const std::vector<HuffmanTreeBasic::Note> &alphabet) {
	byte symbol = alphabet.size() - 1;
	this->pushInBitBuff(symbol);

	for (size_t pos = 0; pos < alphabet.size(); ++pos) {

		this->pushInBitBuff(alphabet[pos].symbol);
		this->pushInBitBuff(alphabet[pos].value);

	}
}

void HuffmanCoding::archive() {

	assert(this->originalFile.good());

	byte buff[BUFF_SIZE];
	const byte *pStr;

	do {
		this->originalFile.read(reinterpret_cast<char*>(buff), BUFF_SIZE);

		pStr = buff;

		while (pStr != buff + this->originalFile.gcount()){

			this->pushInBitBuff(this->code[*pStr].crbegin(),
								this->code[*pStr].crend(),
								this->code[*pStr].size());

			++pStr;
		}
		
	} while (this->originalFile.good() &&
		this->originalFile.peek() != std::char_traits<std::ifstream>::eof());
}

void HuffmanCoding::specialCase(std::vector<HuffmanTreeBasic::Note> &alphabet) {
	
	byte count = 1;

	if (alphabet.size()) {
	
		this->pushInBitBuff(count);	

		
		this->pushInBitBuff(alphabet.front().symbol);

		this->pushInBitBuff(alphabet.front().value);
	}
	else {
		count = 0;
		this->pushInBitBuff(count);

		this->pushInBitBuff(count);

		uint32_t value = 0;
		this->pushInBitBuff(value);
	}
}

void HuffmanCoding::writeInFile(bool lastTime) {

	if (lastTime) {
		this->archiveFile.write(this->bitBuff, (this->bitBuff.getSize() >> 3) + ((this->bitBuff.getSize()&7)?1:0));

		this->bitBuff.clear();
	}
	else{
		this->archiveFile.write(this->bitBuff, (this->bitBuff.getSize() >> 3));

		bool alignment[8];
		size_t count = (this->bitBuff.getSize() & 7),pos;

		for (pos = 0; pos < count; ++pos)
			alignment[pos] = this->bitBuff.getElement(this->bitBuff.getSize() - count + pos);

		this->bitBuff.clear();

		for (pos = 0; pos < count; ++pos)
			this->pushInBitBuff(alignment[pos]);

	}

	if (this->archiveFile.good() == false) {
		this->archiveFile.close();
		this->originalFile.close();

		throw std::ofstream::failure("Archive file is fail!");
	}

}