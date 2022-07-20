#include "Client.hpp"

Client::Client(int fd, std::string host) : fd(fd), hostname(host), state(PASSWORD), offset_(0) {}

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

void Client::addReply(std::string source, std::string numeric, std::string mes) {
	reply.append(":" + source + " " + numeric + " " + nickname + " " + mes + "\r\n");
}

void Client::addReply(std::string source, std::string mes) {
	//	std::string(":serv.bar") + std::string("001") +
	reply.append(":" + source + " " + nickname + " " + mes + "\r\n");
}

void Client::addReply(std::string mes) {
//	reply.append(":" + getNickname() + " " + mes + "\r\n");
	reply.append(mes + "\r\n");
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
	offset_ = 0;
	reply.clear();
	reply = "";
}

std::string Client::getReply() {
	return reply;
}

void Client::joinChannel(Channel *channel) {
	channel->addUser(this);
	channels.insert(std::make_pair(channel->getName(), channel));
}

void Client::leaveChannel(Channel *channel) {
	std::map<std::string,Channel*>::iterator it = channels.begin();
	std::map<std::string,Channel*>::iterator ite = channels.end();
	for (; it != ite; ++it) {
		if (it->second == channel) {
			channel->removeUser(nickname);
			channels.erase(it);
		}
	}
}

void Client::leaveAllChannels() {
	channels.clear();
}

int Client::getChannelNum() {
	return channels.size();
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

std::string &Client::getNickname() {
	return nickname;
}
std::string &Client::getRealname() {
	return realname;
}
