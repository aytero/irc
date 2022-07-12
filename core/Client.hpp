#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

# include <string>
# include "Socket.hpp"


class Client : public Socket {
	int fd;
	std::string reply;

public:
	Client(int fd);
	~Client();
//	void reply(std::string mes);
	void setReply(std::string mes);
	std::string getReply();
};


#endif //IRC_CLIENT_HPP
