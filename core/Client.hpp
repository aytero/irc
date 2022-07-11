#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

# include <string>
# include "Socket.hpp"

class Client : public Socket {
	int fd;

public:
	Client(int fd);
	~Client();
//	void reply(std::string mes);
};


#endif //IRC_CLIENT_HPP
