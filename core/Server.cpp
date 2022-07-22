#include "Server.hpp"

Server::Server(const char *port, const char *pass) : port(port), password(pass) {

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
	logger::info("IRC server configured successfully");
}

Server::~Server() {
	logger::debug("serv destr\n");
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
	for (int i = 0; i < users.size(); ++i) {
		addEvent(WRITE_EVENT, users[i]->getFd());
	}

}

void Server::initListeningSocket() {
	int opt = 1;

	listeningSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listeningSocket < 0) {
		throw std::runtime_error("Error: socket creation.");
		return;
	}
	if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) == -1) {// size of int?
		throw std::runtime_error("Error: setsockopt");
	}
	address.sin_family = PF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(std::stoi(port));

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
	Client *newClient = new Client(client_fd, getHostname());
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

// add size to read
int Server::request(int fd) {
//	std::string message;
	char buffer[100];
	int lenRead;

	memset(buffer, 0, sizeof buffer);
	lenRead = recv(fd, buffer, 100, 0);
	if (lenRead > 0) {
		logger::debug(SSTR("fd: " << fd << " read: " << buffer));
		clients[fd]->addRequest(buffer);
//		message.append(buffer);
		if (!std::strstr(buffer, "\r\n"))
			return NEED_MORE;
		clients[fd]->clearReply();
		commandHandler->handle(clients[fd], clients[fd]->getRequest());
		clients[fd]->clearRequest();
		return DONE_READING;
	}
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

int Server::response(int fd, unsigned dataSize) {
	int lenSent;
	std::string repl = clients[fd]->getReply();

	if (repl.size() <= dataSize) {
		lenSent = send(fd, repl.c_str(), repl.size(), 0);
		clients[fd]->clearReply();
		// clearRequest();
	} else {
		unsigned offset = clients[fd]->getOffset();
		lenSent = send(fd, repl.c_str() + offset, dataSize, 0);
//		lenSent = send(client->getFd(), client->getResponse()->getText() + offset, event.data, 0);
		clients[fd]->setOffset(offset + lenSent);
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

	new_events_num = kevent(kq, 0, 0, eventList, MAX_EVENTS, NULL);
	if (new_events_num == IRC_ERROR) {
		return -1;
	}

	for (int i = 0; i < new_events_num; ++i) {
		struct kevent &event = eventList[i];
		unsigned eventFd = event.ident;
		if (event.flags & EV_EOF)
			disconnectClient(eventFd);
		else if (eventFd == listeningSocket) {
			acceptConnection(event.ident);
		} else if (event.filter == EVFILT_READ) {
//			std::cout << "read event\n";
			int ret = request(event.ident);
			if (ret == DONE_READING)
				addEvent(WRITE_EVENT, eventFd);
			else if (ret == NEED_MORE)
				addEvent(READ_EVENT, eventFd);
			else
				disconnectClient(eventFd);
		} else if (event.filter == EVFILT_WRITE) {
//			std::cout << "write event\n";
			int ret = response(event.ident, event.data);
			if (ret <= 0)
				disconnectClient(eventFd);
		} else
			disconnectClient(eventFd);
	}
	return IRC_OK;
}

int Server::run() {
	logger::info("Running IRC server");
	for (;;) {
		if (processEvents() == IRC_ERROR) {
			logger::debug("err");
			break;
		}
//		if (terminate || quit) {
//			shutdown();
//			break;
//		}
	}
	return IRC_OK;
}

Channel *Server::createChannel(std::string name, std::string key, Client *client) {
	Channel *channel = new Channel(name, key);
	channels.push_back(channel);
	channel->setOp(client);
	return channel;
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

void Server::shutdown() {
	close(kq);
	disconnectAllClients();
	close(listeningSocket);
}


void Server::disconnectAllClients() {
	client_it it = clients.begin();
	client_it ite = clients.end();
	while (it != ite) {
		delete it->second;
		it->second->leaveAllChannels();
		it = clients.erase(it);
	}
	clients.clear();
}

void Server::disconnectClient(int fd) {
	clients[fd]->leaveAllChannels();
	delete clients[fd];
	clients.erase(fd);
}
