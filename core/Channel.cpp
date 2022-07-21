#include "Channel.hpp"

Channel::Channel(std::string name, std::string key) : name(name), key(key), topic(""), modeN(false) {}
Channel::Channel() :name(""), key("") {}
Channel::Channel(const Channel &ref) : name(ref.name), key(ref.key), banlist(ref.banlist),
						userNum(ref.userNum), maxUserNum(ref.maxUserNum), users(ref.users) {}
Channel::~Channel() {}
Channel &Channel::operator=(const Channel &ref) {
	if (this != &ref) {
		name = ref.name;
		key = ref.key;
		userNum = ref.userNum;
		maxUserNum = ref.maxUserNum;
		users = ref.users;
		banlist = ref.banlist;
	}
	return *this;
}


void Channel::broadcast(std::string mes, Client *exclude) {
	logger::debug("broadcast message: " + mes);
	for (int i = 0; i < users.size(); ++i) {
		if (users[i] != exclude)
			users[i]->addReply(mes);
	}
}

bool Channel::isFull() {
	if (userNum + 1 == maxUserNum)
		return true;
	return false;
}

bool Channel::checkIfBanned(std::string nick) {
	for (int i = 0; i < banlist.size(); ++i) {
		if (banlist[i] == nick)
			return true;
	}
	return false;
}

Client *Channel::findUser(std::string &nick) {
	for (int i = 0; i < users.size(); ++i) {
		if (users[i]->getNickname() == nick)
			return users[i];
	}
	return 0;
}

void Channel::removeUser(std::string &nick) {
	for (int i = 0; i < users.size(); ++i) {
		if (users[i]->getNickname() == nick) {
			users[i] = 0;
			return;
		}
	}
}

bool Channel::isOp(const Client *client) {
	for (int i = 0; i < operators.size(); ++i) {
		if (operators[i] == client)
			return true;
	}
	return false;
}

void Channel::setOp(Client *client) {
	operators.push_back(client);
}

void Channel::setTopic(std::string &topic) {
	this->topic = topic;
}

void Channel::addUser(Client *client) {
	users.push_back(client);
}

std::string &Channel::getName() {
	return name;
}

std::string &Channel::getKey() {
	return key;
}

std::string &Channel::getTopic() {
	return topic;
}

bool Channel::outsideMessageAllowed() {
	return modeN;
}
