#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

# include <string>
//# include "Socket.hpp"
# include "Channel.hpp"
# include "../cmd/replies.hpp"

class Channel;

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

	std::string reply;
	std::vector<Channel*> channels;
	RegistrationState state;
	int fd;

	std::string request;
	unsigned int offset_;

public:
	Client(int fd, std::string host);
	~Client();

	int getFd();
	void addRequest(std::string mes) {request.append(mes);}
	std::string &getRequest() {return request;}
	void clearRequest() {request.clear(); request = "";}
	bool isRegistered();
	void addReply(std::string mes);
	void addReply(std::string source, std::string mes);
	void addReply(std::string source, std::string numeric, std::string mes);
	void setReply(std::string mes);
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
	std::string getNickname();
	std::string getRealname();
	std::string getPrefix();

	void joinChannel(Channel *channel);
	void leaveChannel(Channel *channel);
	int getChannelNum();
	Channel *getChannel(std::string name);

	void welcome();
};


#endif //IRC_CLIENT_HPP
