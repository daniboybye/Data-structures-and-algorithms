/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Daniel Urumov
* @idnumber 45135
* @task 3
* @compiler VC
*
*/
#ifndef __MY_STRING_HEADER_INCLDED_
#define __MY_STRING_HEADER_INCLDED_

#include <iostream>

namespace fmi{
namespace sdp{

class String{
public:
	String();
	String(const char*,size_t size);
	String(const String&);
	String& operator=(const String&);
	~String();

	String& operator+=(const String&);
	const char& operator[](size_t) const;
	char& operator[](size_t);

	size_t getSize() const;
	operator const char*() const;

private:
	char *str;
	size_t size;
};

bool operator==(const String&, const String&);
bool operator!=(const String&, const String&);
String operator+(const String&, const String&);
std::ostream& operator<<(std::ostream&, const String&);

}
}

#endif //__MY_STRING_HEADER_INCLDED_