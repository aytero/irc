#include "Socket.hpp"

Socket::Socket(int fd) : fd(fd) {
}

Socket::~Socket() {
	close(fd);
}

int Socket::getFd() {
	return fd;
}
