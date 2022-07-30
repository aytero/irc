#ifndef BOTDCC_HPP
# define BOTDCC_HPP

# include <string>

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h> // for sleep()
# include <stdio.h>
# include <errno.h>
# include <vector>

# include "../logger/Logger.hpp"

//DCC SEND filename ip port file size

//DCC RESUME filename port position
//DCC ACCEPT filename port position


class BotDCC {
	int sock;
	bool stop;
	void sendFile(const std::string &filename);
	void receiveFile(const std::string &filename, unsigned long fileSize);
public:
	BotDCC();
	BotDCC(std::string port, std::string pass);
	~BotDCC() {}

	void run();
//	void send(const std::string &target, const std::string &filePath);

};


#endif //BOTDCC_HPP
