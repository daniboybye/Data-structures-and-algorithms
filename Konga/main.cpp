#include <iostream>
#include "KongaSet.h"

void menu() {
	KongaSet set;
	
	std::string command;
	std::string name;
	std::string uni;
	int index;
	int secondIndex;

	do {
		std::cin >> command;

		// not checked if index is out of range
		if (command == "append") {
			std::cin >> name >> uni >> index;

			if (!set.append(Student(name, uni), index)) {
				std::cout << "Incompatible people" << std::endl;
			}
		}
		else if (command == "removeFirst") {
			std::cin >> index;

			set.removeFirst(index);
		}
		else if (command == "removeLast") {
			std::cin >> index;

			set.removeLast(index);
		}
		else if (command == "merge") {
			std::cin >> index >> secondIndex;

			if (!set.merge(index, secondIndex)) {
				std::cout << "Incompatible people" << std::endl;
			}
		}
		else if (command == "print") {
			set.print();
		}
		else if (command == "remove") {
			std::cin >> name >> index;
			set.remove(name, index);
		}
		else if (command == "quit") {
			return;
		}
		else {
			std::cout << "Invalid command" << std::endl;
		}
	} while (!set.empty());
}

int main() {

	menu();

	return 0;
}