#include "Server.hpp"

bool quit_sig = false;

Server::Server(const int port, const char *pass) : port(port), password(pass) {

	hostname = "irc.example.com";

	kq = kqueue();
	if (kq == -1) {
		exit(1);
	}
	initListeningSocket();
	if (addEvent(READ_EVENT, listeningSocket) == IRC_ERROR) {
		exit(1);
	}
	commandHandler = new CommandHandler(this);

	pingTimeout = 60;
	inactivityTimeout = 120;
	operPassword = "8888";

	logger::info("IRC server configured successfully");
}

Server::~Server() {
	logger::debug("serv destr");
	shutdown();
}

int Server::addEvent(int eventType, int fd) {
	struct kevent event;

	memset(&event, 0, sizeof(event));
	if (eventType == READ_EVENT)
		EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, 0);
	else
		EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, 0);
	if (kevent(kq, &event, 1, NULL, 0, NULL) == IRC_ERROR) {
		return IRC_ERROR;
	}
	return IRC_OK;
}

void Server::broadcastEvent(Client *exclude) {
	client_it it = clients.begin();
	client_it ite = clients.end();

	for (; it != ite; ++it) {
		if (it->second != exclude)
			addEvent(WRITE_EVENT, it->second->getFd());
	}
}

void Server::broadcastEvent(std::vector<Client *> users) {
	for (unsigned int i = 0; i < users.size(); ++i) {
		addEvent(WRITE_EVENT, users[i]->getFd());
	}
}

void Server::broadcastMessage(const std::string &mes) {
	client_it it = clients.begin();
	client_it ite = clients.end();

	for (; it != ite; ++it) {
		it->second->addReply(hostname, mes);
	}
//		if (it->second != exclude)
}

void Server::initListeningSocket() {
	int opt = 1;

	listeningSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listeningSocket < 0) {
		// todo use returns
		throw std::runtime_error("Error: socket creation.");
		return;
	}
	if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) == -1) {// size of int?
		throw std::runtime_error("Error: setsockopt");
	}
	address.sin_family = PF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	if (bind(listeningSocket, (struct sockaddr *)&address, sizeof(address))) {
		throw std::runtime_error("Error bind.");
	}
	if (listen(listeningSocket, 100)) {//serv.max_listen
		throw std::runtime_error("Error listen.");
	}
}

int Server::acceptConnection(int event_fd) {
	struct sockaddr_in client_addr;
	int addrlen = sizeof client_addr;
	int client_fd;

	if ((client_fd = ::accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0) {
		return IRC_ERROR;
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	if (addEvent(READ_EVENT, client_fd) == IRC_ERROR) {
		close(client_fd);
		return IRC_ERROR;
	}
    logger::info(SSTR("IP address is: " << inet_ntoa(client_addr.sin_addr)));
	Client *newClient = new Client(client_fd, inet_ntoa(client_addr.sin_addr));
	clients.insert(std::pair<int,Client*>(newClient->getFd(), newClient));
	logger::debug(SSTR("accepted client with fd " << client_fd));
	return IRC_OK;
}

//std::string &Server::read(int fd) {
//	char *buffer;
//	ssize_t lenRead;
//	size_t bufferLen = event.data; // get from config of the server
//	int event_fd = event.ident;

//	buffer = new char [bufferLen];
//	lenRead = recv(event_fd, buffer, bufferLen, 0);
//	if (lenRead > 0) {
//		logger::debug(buffer);
//		session->reqParser(buffer, bufferLen);
//	}
//	delete [] buffer;
//	return lenRead;
//}


void Server::welcome(Client *client) {
	const std::string &nick = client->getNickname();

	client->addReply(hostname, RPL_WELCOME(nick, client->getPrefix()));
	client->addReply(hostname, RPL_YOURHOST(nick, client->getHostname(), std::string("1.1")));
//	client->addReply(hostname, RPL_CREATED(nick, server_->getHostname())); // date of server creation
	client->addReply(hostname, RPL_MYINFO(nick, hostname, std::string("1.1"),
										  std::string("oOa"), std::string("O")));

//	client->addReply(hostname, RPL_LUSERCLIENT());
//	client->addReply(hostname, RPL_LUSEROP(std::to_string(server_->get)));
//	client->addReply(hostname, RPL_LUSERUNKNOWN()); //
	client->addReply(hostname, RPL_LUSERCHANNELS(nick, std::to_string(getChannelNum())));
}

void Server::checkConnectionRegistration(Client *client) {
	if (client->getNickname().size() > 0 && client->getUsername().size() > 0) {
		std::string mes = "MOTD";
		client->setState(DONE);
		welcome(client);
		commandHandler->handle(client, mes);
	}
}

int Server::request(int fd) {
	char *buffer;
	int lenRead;

	unsigned bufferLen = 200;

	buffer = new char [bufferLen];
	memset(buffer, 0, bufferLen);

	lenRead = recv(fd, buffer, bufferLen, 0);
	logger::debug(SSTR("lenRead: " << lenRead));
	if (lenRead > 0) {
		logger::debug(SSTR("fd: " << fd << " read: " << buffer));
		clients[fd]->addRequest(buffer, lenRead);
		if (!std::strstr(buffer, "\n")) {
			delete[] buffer;
			return NEED_MORE;
		}
		delete[] buffer;
		clients[fd]->clearReply();
		commandHandler->handle(clients[fd], clients[fd]->getRequest());
		clients[fd]->clearRequest();
		return DONE_READING;
	}
	delete[] buffer;
	return BAD_REQUEST;
//	while (!std::strstr(buffer, "\r\n")) {
//		if (recv(fd, buffer, 100, 0) > 0) {
//			message.append(buffer);
//		} else {
//			std::cout << "some err\n";
////			break;
//		}
//	}
//	return IRC_OK;
}

// todo: better use fixed data size
int Server::response(int fd, unsigned int dataSize) {
	int lenSent;

	// check if exists
	// error here
	std::string repl = clients.at(fd)->getReply();

	/// change repl size ?
	if (repl.size() <= dataSize) {
		lenSent = send(fd, repl.c_str(), repl.size(), 0);
		clients[fd]->clearReply();
		// clearRequest();
	} else {
		unsigned offset = clients[fd]->getOffset();
		lenSent = send(fd, repl.c_str() + offset, dataSize, 0);
//		lenSent = send(client->getFd(), client->getResponse()->getText() + offset, event.data, 0);
		clients[fd]->setOffset(offset + lenSent);
		clients[fd]->setReply(repl.substr(lenSent));
		if (lenSent > 0)
			addEvent(WRITE_EVENT, fd);
	}
	return lenSent;
}

//int Server::response(int fd) {
//	int lenSent;
//
//	std::string repl = clients[fd]->getReply();
//	lenSent = send(fd, repl.c_str(), repl.size(), 0);
//	return lenSent;
//}

int Server::processEvents() {
	int new_events_num;
	struct kevent eventList[MAX_EVENTS];
	struct timespec tmout = {
		61, // block for 60 seconds at most
		0 // nanoseconds
	};

	new_events_num = kevent(kq, 0, 0, eventList, MAX_EVENTS, &tmout);
	if (new_events_num == IRC_ERROR) {
		return -1;
	} else if (new_events_num == 0) {
		logger::warn("No events; timeout");
	}

	for (int i = 0; i < new_events_num; ++i) {
		struct kevent &event = eventList[i];
		unsigned eventFd = event.ident;

		if (event.flags & EV_EOF) {
			disconnectClient(eventFd);
		}
		else if (eventFd == listeningSocket) {
			acceptConnection(event.ident);
		}
		else if (event.filter == EVFILT_READ) {
			int ret = request(event.ident);
			if (ret == DONE_READING) {
				logger::debug("done reading");
				addEvent(WRITE_EVENT, eventFd);
			}
			else if (ret == NEED_MORE)
				addEvent(READ_EVENT, eventFd);
			else
				; // ignore
			// disconnectClient(eventFd);
			clients[eventFd]->updateLastActivityTime();
		}
		else if (event.filter == EVFILT_WRITE) {
			//				logger::debug("write event");
			int ret = response(event.ident, event.data);
			if (ret < 0)// <=
				logger::warn("sending response failed");
//				disconnectClient(eventFd);
			clients[eventFd]->updateLastActivityTime();
		}// else
		//			disconnectClient(eventFd);
	}
//	logger::info("after events");
//	}
	return IRC_OK;
}


void Server::pingConnection() {
	client_it it = clients.begin();
	client_it ite = clients.end();
//	logger::info("Ping func");
	for (; it != ite; ++it) {
//		logger::info("Ping loop");
		if (it->second->isRegistered()) {
			if (std::time(0) - it->second->getLastActivityTime() > static_cast<time_t>(inactivityTimeout)) {
//				std::string cmd = "PING";
//				commandHandler->handle(it->second, cmd);
				it->second->addReply(hostname, RPL_PING(hostname));
				it->second->updateLastPingTime();
				it->second->updateLastActivityTime();
				// set state on pinging
				it->second->statePing(true);
				addEvent(WRITE_EVENT, it->first);
				logger::info(SSTR("Ping to user " << it->first));

			}
			if (it->second->pinging() && std::time(0) - it->second->getTimeAfterPing() > static_cast<time_t>(pingTimeout))
				it->second->quit();
		}
	}


}

void sigHandler(int) {
	quit_sig = true;
}

int Server::run() {
	logger::info("Running IRC server");

	// todo sigint sigpipe
	signal(SIGINT, sigHandler);
	for (;;) {
		try {
			if (processEvents() == IRC_ERROR) {
				logger::debug("err");
//				break;
			}
		} catch (const char *e) {
			return IRC_OK;
		}
		if (quit_sig) {
			logger::info("Signal termination");
			break;
		}
		pingConnection();
		// todo too slow if client reconnects
		deleteClosedSessions();
		deleteEmptyChannels();

	}
	return IRC_OK;
}

Channel *Server::createChannel(std::string name, std::string key, Client *client) {
	Channel *channel = new Channel(name, key);
	channels.push_back(channel);
	channel->setOp(client);
	return channel;
}

//void Server::deleteChannel(const std::string &name) {
//	std::vector<Channel*>::iterator it = channels.begin();
//	for (; it != channels.end(); ++it) {
//		if ((*it)->getName() == name) {
//			delete *it;
//			channels.erase(it);
//			break;
//		}
//	}
//}

void Server::deleteEmptyChannels() {
	std::vector<Channel*>::iterator it = channels.begin();
	while (it != channels.end()) {
		if ((*it)->getUserNum() == 0) {
			delete *it;
			it = channels.erase(it);
		} else
			++it;
	}
}

Channel *Server::getChannel(std::string name) {
	for (unsigned long i = 0; i < channels.size(); ++i) {
		if (channels[i]->getName() == name)
			return channels[i];
	}
	return 0;
}

int Server::getChannelNum() {
	return channels.size();
}

Client *Server::getClient(std::string nick) {
	std::map<int,Client*>::iterator it = clients.begin();
	std::map<int,Client*>::iterator ite = clients.end();
	for (; it != ite; ++it) {
		if (it->second->getNickname() == nick)
			return it->second;
	}
	return 0;
}

bool Server::isOp(std::string &nick) {
	for (unsigned int i = 0; i < opers.size(); ++i) {
		if (opers[i] == nick)
			return true;
	}
	return false;
}

void Server::addOper(std::string &nick) {
	opers.push_back(nick);
}

void Server::ban(std::string &nick) {
	banlist.push_back(nick);
}

bool Server::isBanned(std::string &nick) {
	for (unsigned int i = 0; i < banlist.size(); ++i) {
		if (banlist[i] == nick)
			return true;
	}
	return false;
}

void Server::shutdown() {
	logger::info("Shutting down...");
	close(kq);
	delete commandHandler;

	disconnectAllClients();


	std::vector<Channel*>::iterator it = channels.begin();
	while (it != channels.end()) {
		delete *it;
		++it;
//		it = channels.erase(it);
	}
	channels.clear();

	close(listeningSocket);
}


void Server::disconnectAllClients() {
	client_it it = clients.begin();
	client_it ite = clients.end();
	while (it != ite) {
		delete it->second;
		it = clients.erase(it);
	}
	clients.clear();
}

void Server::deleteClosedSessions() {
	client_it it = clients.begin();
	client_it ite = clients.end();
	while (it != ite) {
		if (it->second->haveQuit()) {
			delete it->second;
			it = clients.erase(it);
		} else
			++it;
	}
}

void Server::disconnectClient(int fd) {
	logger::info(SSTR("User " << clients[fd]->getNickname() << " fd: " << fd << " disconnected"));
	delete clients[fd];
	clients.erase(fd);
}
