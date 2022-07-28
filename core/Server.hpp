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

# include <ctime>


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

	std::vector<std::string> opers;
	std::string operPassword;
	std::vector<std::string> banlist;

//	Event *event;
	std::map<int, Client*> clients;
	std::vector<Channel*> channels;
	CommandHandler *commandHandler;

//	Server();
	Server(const Server &);
	Server& operator=(const Server &);

	void initListeningSocket();
	int acceptConnection(int event_fd);

	void disconnectAllClients();
	void shutdown();

	int processEvents();

	int request(int fd);
	int response(int fd, unsigned int dataSize);

	typedef std::map<int,Client*>::iterator client_it;

	unsigned long long ping_delay;
	time_t last_ping;
//		std::time_t t = std::time(0);   // get time now
//		std::tm *now = std::localtime(&t);
//		std::cout << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << "\n";

	void sendPing();

public:
	Server(const char *port, const char *pass);
	~Server();
	int run();

	void broadcastEvent(Client *exclude = 0);
	void broadcastEvent(std::vector<Client*> users);
	int addEvent(int eventType, int fd);
	Channel *createChannel(std::string name, std::string key, Client *client);
	Channel *getChannel(std::string name);
	std::vector<Channel*> getChannels() {return channels;}
	int getChannelNum();

	Client *getClient(std::string nick);
	void disconnectClient(int fd);
	std::string &getHostname() {return hostname; }
	std::string &getOperPassword() {return operPassword; }
	bool isOp(std::string &nick);
	void addOper(std::string &nick);
	void ban(std::string &nick);
	bool isBanned(std::string &nick);
//	void welcome(Client *client);

	enum returnStatus {
		IRC_OK = 0,
		IRC_ERROR = -1,
	};
};


#endif //IRC_SERVER_HPP
