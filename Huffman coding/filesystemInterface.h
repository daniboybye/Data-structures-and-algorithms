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

#ifndef __FILESYSTEM_INTERFACE_HEADER_INCLUDED_
#define __FILESYSTEM_INTERFACE_HEADER_INCLUDED_

#include <string>
#include <vector>
#include <cassert>

namespace fmi {
namespace sdp {
namespace structures {


class FilesystemInterface {
public:
	FilesystemInterface(const char *str);

	virtual ~FilesystemInterface();

	FilesystemInterface(const FilesystemInterface&) = default;
	
	FilesystemInterface& operator=(const FilesystemInterface&) = default;

	virtual bool isDirectory() const = 0;
	
	virtual void print(std::ostream &os, int tab=0) const = 0 {

		while (tab-- > 0)
			os << "  ";
	}

	const std::string& getName() const {
		return this->name;
	}

private:
	std::string name;
};

class File
	:public FilesystemInterface {

public:
	File(const char *str);

	virtual ~File();

	File(const File&) = default;

	File& operator=(const File&) = default;

	virtual bool isDirectory() const {
		return false;
	}

	virtual void print(std::ostream &os, int tab=0) const {
		this->FilesystemInterface::print(os, tab);
		os << '-' << this->getName() << std::endl;
	}
};

class Directory
	:public FilesystemInterface {

public:
	Directory(const char *str);

	virtual ~Directory();

	Directory(const Directory&) = default;

	Directory& operator=(const Directory&) = default;

	virtual bool isDirectory() const {
		return true;
	}

	const std::vector<FilesystemInterface*>& getSubdirectoriesAndFiles() const {
		return this->subdirectoriesAndFiles;
	}

	void add(FilesystemInterface *value) {
		this->subdirectoriesAndFiles.push_back(value);
	}

	size_t getSize() const {
		return this->subdirectoriesAndFiles.size();
	}

	virtual void print(std::ostream &os, int tab = 0) const;

	FilesystemInterface* operator[](int index) const{
		assert(index >= 0 && index < this->subdirectoriesAndFiles.size());

		return this->subdirectoriesAndFiles[index];
	}

private:
	std::vector<FilesystemInterface*> subdirectoriesAndFiles;
};

}//namespace structures
}//namespace sdp
}//namespace fmi


#endif //__FILESYSTEM_INTERFACE_HEADER_INCLUDED_