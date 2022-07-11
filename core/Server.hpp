#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>

# include <sys/event.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>

# include "Event.hpp"
# include "Client.hpp"
# include "cmd/CommandHandler.hpp"

class Server {
	int kq;
	int fd;
	unsigned listeningSocket;
	struct sockaddr_in address;
	const std::string port;
	const std::string password;

//	Event *event;
	std::map<int, Client*> clients;
//	std::vector<Channel*> channels;
	CommandHandler *commandHandler;

	Server(const Server &);
	Server& operator=(const Server &);

	void initListeningSocket();
	int acceptConnection(int event_fd);

	void disconnectClient(int fd);
	void disconnectAllClients();
	void shutdown();

	int processEvents();

	int addReadEvent(int fd);
	int addWriteEvent(int fd);
	int request(int fd);
	int response(const struct kevent &event, Client *client);

public:
	Server();
	Server(const char *port, const char *pass);
	~Server();
	int run();

	enum returnStatus {
		IRC_OK,
		IRC_ERROR,
	};
};


#endif //IRC_SERVER_HPP
