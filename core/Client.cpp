#include "Client.hpp"

Client::Client(int fd) : Socket(fd) {}

Client::~Client() {}

//void Client::reply(std::string mes) {
//	int lenSent;
//
////	clients[fd]->response();
//	lenSent = send(fd, mes, mes.size(), 0);
////	return lenSent;
//}

void Client::setReply(std::string mes) {
	reply = mes;
}
std::string Client::getReply() {
	return reply;
}
