#include "Client.hpp"

Client::Client(int fd, std::string host) : fd(fd), hostname(host), state(PASSWORD), offset_(0), opername("") {

	quit_ = false;

	mode['a'] = false;
	mode['i'] = false;
	mode['w'] = false;
	mode['r'] = false;
	mode['o'] = false;
	mode['O'] = false;
}

Client::~Client() {
	logger::debug(SSTR("client destr, fd " << fd));
	leaveAllChannels();
	close(fd);
}

int Client::getFd() {
	return fd;
}
bool Client::isRegistered() {
	return state == DONE;
}

void Client::addReply(std::string from, std::string mes) {
	std::string m = ":" + from + " " + mes +  "\r\n";
	reply.append(m);
	logger::debug(SSTR("to: " << nickname << " message: " << mes));
}

//void Client::addReply(std::string from, std::string mes) {
	//	std::string(":serv.bar") + std::string("001") +
//	reply.append(":" + from + " " + nickname + " " + mes + "\r\n");
//}

void Client::addReply(std::string mes) {
//	reply.append(":" + getNickname() + " " + mes + "\r\n");
	reply.append(mes + "\r\n");
	logger::debug(SSTR("to: " << nickname << " message: " << mes));
}

void Client::welcome() {
	if (state != DONE)
		return;
	addReply(RPL_WELCOME(nickname, getPrefix()));
	//	addReply(sprintf("Nickname is set to %s ", nickname));
}

std::string Client::getPrefix() {
	return nickname + (username.empty() ? "" : "!" + username) + (hostname.empty() ? "" : "@" + hostname);
}

//void Client::setReply(std::string mes) {
//	reply = mes;
//}

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
	chan_it it = channels.begin();
	chan_it ite = channels.end();
	while (it != ite) {
		if (it->second == channel) {
			channel->removeUser(nickname);
			it = channels.erase(it);
		} else
			++it;
	}
}

void Client::leaveAllChannels() {
	chan_it it = channels.begin();
	chan_it ite = channels.end();
	while (it != ite) {
		it->second->removeUser(nickname);
		it = channels.erase(it);
	}
	channels.clear();
}

void Client::addRequest(const char *mes, int len) {
	request.append(mes, len);
}

int Client::getChannelNum() {
	return channels.size();
}

Channel *Client::getChannel(std::string name) {
	chan_it chan = channels.find(name);
	if (chan == channels.end())
		return 0;
	return chan->second;
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

void Client::setOpername(std::string &name) {
	opername = name;
}

std::string &Client::getOpername() {
	return opername;
}

std::string &Client::getNickname() {
	return nickname;
}

std::string &Client::getRealname() {
	return realname;
}

std::string &Client::getHostname() {
	return hostname;
}

bool Client::switchMode(const char m, bool toggle) {
	std::map<char,bool>::iterator it = mode.find(m);
	if (it != mode.end()) {
		it->second = toggle;
		return true;
	}
	return false;
//	mode[m] = toggle;
}

std::string Client::getModeStr() {
	std::map<char,bool>::iterator it = mode.begin();
	std::map<char,bool>::iterator ite = mode.end();
	std::string mode_str = "";

	for (; it != ite; ++it) {
		if (it->second)
			mode_str.push_back(it->first);
	}
	return mode_str;
}
