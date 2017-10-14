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
#ifndef __VECTOR_PROXY_HEADER_INCLUDED_
#define __VECTOR_PROXY_HEADER_INCLUDED_

class VectorProxy{

friend class Vector;

public:
	operator const DataType&() const {
		return this->vector.vector[pos];
	}

	const DataType& operator=(const DataType &value) {
		vector.vector[this->pos] = value;

		if (vector.isSorted()) vector.move(this->pos);

		return *this;
	}

	const DataType& operator=(const VectorProxy &other) {
		return this->operator=((const DataType&)other);
	}

private:
	Vector &vector;
	size_t pos;

	VectorProxy(Vector &vector, size_t pos)
		:vector(vector)
		, pos(pos) {}

};

#endif //__VECTOR_PROXY_HEADER_INCLUDED_