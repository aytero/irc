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
# include <fcntl.h>
# include <unistd.h>

# include "Client.hpp"
# include "Channel.hpp"
# include "cmd/CommandHandler.hpp"
# include "cmd/Command.hpp"

# define MAX_EVENTS 128

class Command;
class CommandHandler;

enum EventType {
	READ_EVENT,
	WRITE_EVENT
};

class Server {
	int kq;
//	int fd;
	unsigned listeningSocket;
	struct sockaddr_in address;
	const std::string port;
	const std::string password;

//	Event *event;
	std::map<int, Client*> clients;
	std::vector<Channel*> channels;
	CommandHandler *commandHandler;

//	Server();
	Server(const Server &);
	Server& operator=(const Server &);

	void initListeningSocket();
	int acceptConnection(int event_fd);

	void disconnectClient(int fd);
	void disconnectAllClients();
	void shutdown();

	int processEvents();

	int addEvent(int eventType, int fd);
	int request(int fd);
	int response(int fd);

public:
	Server(const char *port, const char *pass);
	~Server();
	int run();

	void createChannel(std::string name, str::string key, Client *client);
	Channel *getChannel(str::string name);
	int getChannelNum();

	enum returnStatus {
		IRC_OK = 0,
		IRC_ERROR = -1,
	};
};


#endif //IRC_SERVER_HPP
