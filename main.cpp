#include <iostream>

#include "core/Server.hpp"
#include "logger/Logger.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: ./app <port> <password>" << std::endl;
		return 1;
	}

	int port = atoi(argv[1]);

	if (port < 1024 || port > 49151) {
		std::cout << "Error: valid port range is 1024 - 49151" << std::endl;
		return 1;
	}

	logger::setLogLevel(logger::DEBUG);
	Server *serv = new Server(port, argv[2]);

	serv->run();
	delete serv;

	logger::info("fin.");

	return 0;
}