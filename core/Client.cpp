#include "Client.hpp"

Client::Client(int fd, std::string host) : fd(fd), hostname(host), state(PASSWORD) {}

Client::~Client() {
	std::cout << "client destr\n";
	close(fd);
}

int Client::getFd() {
	return fd;
}
bool Client::isRegistered() {
	return state == DONE;
}

void Client::addReply(std::string serv, std::string mes) {
	//	std::string(":serv.bar") + std::string("001") +
	std::cout << "serv repl\n";
	reply.append(":" + serv + " " + nickname + " " + mes + "\r\n");
}

void Client::addReply(std::string mes) {
	reply.append(":" + getNickname() + " " + mes + "\r\n");
}

void Client::welcome() {
	if (state != DONE)
		return;
	addReply(RPL_WELCOME(nickname));
	//	addReply(sprintf("Nickname is set to %s ", nickname));
}

std::string Client::getPrefix() {
	return nickname + (username.empty() ? "" : "!" + username) + (hostname.empty() ? "" : "@" + hostname);
}

void Client::setReply(std::string mes) {
	reply = mes;
}

void Client::clearReply() {
	reply.clear();
}

std::string Client::getReply() {
	return reply;
}

void Client::setState(RegistrationState new_state) {
	state = new_state;
}

RegistrationState Client::getState() {
	return state;
}

void Client::setPassword(std::string &pass) {
	password = pass;
}

void Client::setNickname(std::string &nick) {
	nickname = nick;
}

void Client::setUsername(std::string &name) {
	username = name;
}

void Client::setRealname(std::string &name) {
	realname = name;
}

std::string Client::getNickname() {
	return nickname;
}
std::string Client::getRealname() {
	return realname;
}
