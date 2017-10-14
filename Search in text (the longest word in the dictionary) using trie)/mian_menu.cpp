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
#include <iostream>
#include <fstream>
#include <ctime>
#include "vector.hpp"
#include "string.h"
#include "dictionary.h"

int main(int argc, char *argv[]) {


	if (argc < 2) return -1;

	Dictionary dictionary;
	dictionary.addFileWithWords(argv[1]);

	for (int i = 2; i < argc; ++i) {
		dictionary.calculateFileCoefficient(argv[i]);
	}


	return 0;
}