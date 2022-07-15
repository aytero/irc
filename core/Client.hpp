#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

# include <string>
# include "Socket.hpp"
# include "Channel.hpp"


class Client : public Socket {
	std::string nickname;
	std::string realName;

	std::string reply;
	std::vector<Channel*> channels;

public:
	Client(int fd);
	~Client();
//	void reply(std::string mes);
	void setReply(std::string mes);
	std::string getReply();
	std::string getNickname();
	std::string getRealName();

	std::string getPrefix();
	void joinChannel(Channel *channel);
	void leaveChannel(Channel *channel);
	int getChannelNum();
	Channel *getChannel(std::string name);
};


#endif //IRC_CLIENT_HPP
