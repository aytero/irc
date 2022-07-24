#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

class Client;

# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>

# include <string>
# include <iostream>
# include <map>

# include "../logger/Logger.hpp"
//# include "Socket.hpp"
# include "Channel.hpp"
# include "../cmd/replies.hpp"

enum RegistrationState {
//	START,
	PASSWORD,
	NICKNAME,
	USERNAME,
	DONE,
};

enum RequestState {
	BAD_REQUEST,
	NEED_MORE,
	DONE_READING,
};

class Client {
	std::string password;
	std::string nickname; // appears for other users
	std::string username; // used to log in
	std::string realname;
	std::string hostname;

	std::string opername; // use when perform oper cmds

	std::string reply;
	std::map<std::string, Channel*> channels;
	RegistrationState state;
	std::map<char,bool> mode;

	int fd;

	std::string request;
	unsigned int offset_;
	bool quit_;

	typedef std::map<std::string,Channel*>::iterator chan_it;

public:
	Client(int fd, std::string host);
	~Client();

	int getFd();
	void addRequest(const char *mes, int len);
	std::string &getRequest() {return request;}
	void clearRequest() {request.clear(); request = "";}
	bool isRegistered();
	void addReply(std::string mes);
	void addReply(std::string from, std::string mes);
	//void addReply(std::string source, std::string numeric, std::string mes);
//	void setReply(std::string mes);
	void clearReply();
	std::string getReply();

	void setOffset(unsigned int offset) {offset_ = offset;}
	unsigned int getOffset() { return offset_; }

	void setState(RegistrationState new_state);
	RegistrationState getState();

	void setPassword(std::string &pass);
	void setNickname(std::string &nick);
	void setUsername(std::string &name);
	void setRealname(std::string &name);
	void setOpername(std::string &name);
	std::string &getNickname();
	std::string &getRealname();
	std::string &getHostname();
	std::string &getOpername();
	std::string getPrefix();

	void joinChannel(Channel *channel);
	void leaveChannel(Channel *channel);
	void leaveAllChannels();
	int getChannelNum();
	Channel *getChannel(std::string name);

	void quit() { quit_ = true;}
	bool haveQuit() { return quit_;}

	void welcome();

	bool switchMode(const char m, bool toggle);
	std::string getModeStr();

};


#endif //IRC_CLIENT_HPP
