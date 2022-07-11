#include <iostream>
#include "core/Server.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "should be 2 args";
		return 1;
	}
	Server *serv = new Server(argv[1], argv[2]);
	serv->run();
	return 0;
}