#include <iostream>
#include "core/Server.hpp"
#include "logger/Logger.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "should be 2 args";
		return 1;
	}
	logger::setLogLevel(logger::DEBUG);
	Server *serv = new Server(argv[1], argv[2]);
	serv->run();
	delete serv;
	return 0;
}