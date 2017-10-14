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
#include <ctime>

#include "HuffmanAlgorithm.h"


int main(int argc,char *argv[]) {

	clock_t s = clock(), e;

	try {
		fmi::sdp::compression::HuffmanAlgorithm alg;

		if (argc == 3 && strcmp(argv[1], "-List") == 0)
			alg.list(argv[2]);
		
		else if(argc==4 && strcmp(argv[1],"-Pack")==0) 
			alg.pack(argv[2],argv[3]);
		
		else if (argc == 4 && strcmp(argv[1], "-Unpack") == 0)
			alg.unpack(argv[2], argv[3]);

		else if (argc == 4 && strcmp(argv[1], "-Search") == 0)
			alg.search(argv[2], argv[3]);
		
		else 
			std::cout<<"Invalid instruction"<<std::endl;
		
		

	/*	alg.list("M:\my_archive.arc");
		alg.search("M:\my_archive.arc", "AWESOME");
		;*/

		/*alg.unpack("M:\\archive.arc", "M:\\LEVEL\\");*/

	}
	catch (const std::bad_alloc &exception) {
		std::cerr << exception.what() << std::endl;
	}
	catch (const std::ofstream::failure &exception) {
		std::cerr << exception.what() << std::endl;
	}
	catch (const boost::filesystem::filesystem_error &exception) {
		std::cerr << exception.what() << std::endl;
		std::cerr << "boost is fail" << std::endl;
	}
	catch (const std::string &exception) {
		std::cerr << exception << std::endl;
	}


	e = clock();

	std::cout << std::endl;
	std::cout << "Time : "<< ((double)(e - s) / CLOCKS_PER_SEC) << std::endl;


	return 0;
}