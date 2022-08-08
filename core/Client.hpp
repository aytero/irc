#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

class Client;

# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>

# include <ctime>
# include <string>
# include <iostream>
# include <map>

# include "../logger/Logger.hpp"
//# include "Socket.hpp"
# include "Channel.hpp"
# include "../cmd/replies.hpp"

enum UserState {
//	START,
//	PASSWORD,
//	USERNAME,
//    NICKNAME,
	PENDING,
	DONE,
    QUIT,
};

enum RequestState {
	BAD_REQUEST,
	NEED_MORE,
	DONE_READING,
};

class Client {
	int fd;
	std::string nickname; // appears for other users
	std::string username; // used to log in
	std::string realname;
	std::string password;
	std::string hostname;

	std::string opername; // use when perform oper cmds

	std::map<std::string, Channel*> channels;
	UserState state;
	std::map<char,bool> mode;

	time_t lastPingTime;
	time_t lastActivityTime;
	// registration time
	bool pinging_;

	std::vector<char> request_bytes;
	std::string request;
	unsigned int offset_;
	std::string reply;

	typedef std::map<std::string,Channel*>::iterator chan_it;

public:
	Client(int fd, std::string host);
	~Client();

	const time_t &getLastActivityTime() const {
		return lastActivityTime;
	}
	const time_t &getTimeAfterPing() const {
		return lastPingTime;
	}
	void updateLastActivityTime() {
		lastActivityTime = std::time(0);
	}
	void updateLastPingTime() {
		lastPingTime = std::time(0);
	}
	bool pinging() {return pinging_; }
	void statePing(bool toggle = true) {
		pinging_ = toggle;
	}

	int getFd();
	void addRequest(const char *mes, int len);
	std::vector<char> &getRequestBytes() {return request_bytes;}
	std::string &getRequest() {
		std::string r(request_bytes.begin(), request_bytes.end());
		request = r;
		return request;
	}
	//std::string &getRequest() {return request;}
	void clearRequest() {request_bytes.clear(); request.clear(); request = "";}
	//void clearRequest() {request.clear(); request = "";}
	bool isRegistered();
	void addReply(std::string mes);
	void addReply(std::string from, std::string mes);
	void addReply(const std::string &from, const std::string numeric, const std::string &mes);
	void setReply(std::string mes);
	void clearReply();
	std::string getReply();

	void setOffset(unsigned int offset) {offset_ = offset;}
	unsigned int getOffset() { return offset_; }

	void setState(UserState new_state);
	UserState getState();

	void setPassword(std::string pass);
	void setNickname(std::string nick);
	void setUsername(std::string name);
	void setRealname(std::string name);
	void setOpername(std::string name);
	std::string &getNickname();
	std::string &getUsername();
	std::string &getRealname();
	std::string &getHostname();
	std::string &getOpername();
	std::string getPrefix();

	int joinChannel(Channel *channel);
	void leaveChannel(Channel *channel);
	void leaveAllChannels();
	int getChannelNum();
	Channel *getChannel(std::string name);
	std::map<std::string,Channel*> getAllChannels() { return channels; }

	void quit() { state = QUIT; }
	bool haveQuit() { return state == QUIT; }


	void welcome();

	bool switchMode(const char m, bool toggle);
	std::string getModeStr();

};


#endif //IRC_CLIENT_HPP
