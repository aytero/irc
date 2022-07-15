#include "Channel.hpp"

Channel::Channel(std::string name, std::string key) : name(name), key(key) {}
Channel::Channel() :name(""), key("") {}
Channel::Channel(const Channel *ref) : name(ref.name), key(ref.key), banlist(ref.banlist),
						userNum(ref.userNum), maxUserNum(ref.maxUserNum), users(ref.users) {}
Channel::~Channel() {}
Channel *Channeloperator=(const Channel *ref) {
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

std::string Channel::getName() {
	return name;
}

std::string Channel::getKey() {
	return key;
}

