#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

class Server;

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


# include "logger/Logger.hpp"
//# include "Client.hpp"
# include "Channel.hpp"
# include "../cmd/CommandHandler.hpp"

# define MAX_EVENTS 128

//class CommandHandler;
//class Channel;

enum EventType {
	READ_EVENT,
	WRITE_EVENT
};

class Server {
	int kq;
	unsigned listeningSocket;
	struct sockaddr_in address;
	const std::string port;
	const std::string password;
	std::string hostname;

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

	int request(int fd);
	int response(int fd, unsigned dataSize);

	typedef std::map<int,Client*>::iterator client_it;

public:
	Server(const char *port, const char *pass);
	~Server();
	int run();

	void broadcastEvent(Client *exclude = 0);
	int addEvent(int eventType, int fd);
	Channel *createChannel(std::string name, std::string key, Client *client);
	Channel *getChannel(std::string name);
	int getChannelNum();

	Client *getClient(std::string nick);
	std::string &getHostname() {return hostname; }

	enum returnStatus {
		IRC_OK = 0,
		IRC_ERROR = -1,
	};
};


#endif //IRC_SERVER_HPP
