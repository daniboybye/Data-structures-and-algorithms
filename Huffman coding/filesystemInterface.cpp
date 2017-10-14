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

#include "filesystemInterface.h"

using namespace fmi::sdp::structures;

FilesystemInterface::FilesystemInterface(const char *str)
:name(str)
{}

FilesystemInterface::~FilesystemInterface()
{}

File::File(const char *str)
:FilesystemInterface(str)
{}

File::~File() 
{}

Directory::Directory(const char *str)
:FilesystemInterface(str)
, subdirectoriesAndFiles()
{}

Directory::~Directory() {
	for (auto i = this->subdirectoriesAndFiles.cbegin(); i != this->subdirectoriesAndFiles.cend(); ++i)
		delete (*i);
}

void Directory::print(std::ostream &os, int tab) const {

	this->FilesystemInterface::print(os, tab);
	os << '+' << this->getName() << std::endl;

	this->FilesystemInterface::print(os, tab);
	os << "{" << std::endl;

	for (auto i = this->subdirectoriesAndFiles.cbegin(); i != this->subdirectoriesAndFiles.cend(); ++i)
		(*i)->print(os, tab + 1);

	this->FilesystemInterface::print(os, tab);
	os << "}" << std::endl;
}