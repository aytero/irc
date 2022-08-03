#include "Channel.hpp"

Channel::Channel(std::string name, std::string key) : name(name), key(key), topic(""), modeN(false), userNum(0), maxUserNum(10) {}
Channel::Channel() : name(""), key("") {}
Channel::Channel(const Channel &ref) : name(ref.name), key(ref.key), topic(ref.topic), modeN(ref.modeN),
							userNum(ref.userNum), maxUserNum(ref.maxUserNum), users(ref.users), banlist(ref.banlist) {}
Channel::~Channel() {}
Channel &Channel::operator=(const Channel &ref) {
	if (this != &ref) {
		name = ref.name;
		key = ref.key;
		topic = ref.topic;
		modeN = ref.modeN;
		userNum = ref.userNum;
		maxUserNum = ref.maxUserNum;
		users = ref.users;
		banlist = ref.banlist;
	}
	return *this;
}


void Channel::broadcast(std::string mes, Client *exclude) {
	logger::debug("broadcast message: " + mes);
	for (unsigned int i = 0; i < users.size(); ++i) {
		if (users[i] != exclude)
			users[i]->addReply(mes);
	}
}

void Channel::kick(Client *op, Client *target, const std::string &reason) {
	// opername or ! creator flag
	std::string mes = RPL_KICK(op->getPrefix(), name, target->getNickname(), reason);

	broadcast(mes);
	logger::debug(mes);
	target->leaveChannel(this);
//	removeUser(target->getNickname());
}

bool Channel::isFull() {
	if (userNum + 1 == maxUserNum)
		return true;
	return false;
}

bool Channel::checkIfBanned(std::string nick) {
	for (unsigned int i = 0; i < banlist.size(); ++i) {
		if (banlist[i] == nick)
			return true;
	}
	return false;
}

Client *Channel::findUser(std::string &nick) {
	for (unsigned int i = 0; i < users.size(); ++i) {
		if (users[i]->getNickname() == nick)
			return users[i];
	}
	return 0;
}

void Channel::removeUser(std::string &nick) {
	std::vector<Client*>::iterator it = users.begin();
	for (; it != users.end(); ++it) {
		if ((*it)->getNickname() == nick) {
//			(*it)->leaveChannel(this);
//			operators.erase(std::find(operators.begin(), operators.end(), *it));
//			std::find(vec.begin(), vec.end(), item) != vec.end()
			users.erase(it);
			--userNum;
//			if (operators.empty() && userNum > 0)
//				operators.push_back(users.front());
			return;
		}
	}
}

bool Channel::isOp(const Client *client) {
	for (unsigned int i = 0; i < operators.size(); ++i) {
		if (operators[i] == client)
			return true;
	}
	return false;
}

void Channel::setOp(Client *client) {
	if (operators.empty())
		creator = client;
	operators.push_back(client);
}

void Channel::setTopic(std::string &topic) {
	this->topic = topic;
}

void Channel::addUser(Client *client) {
	users.push_back(client);
	++userNum;
}


int Channel::getUserNum() {
	return userNum;
}

std::vector<Client*> Channel::getUsers() {
	return users;
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
