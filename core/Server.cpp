#include "Server.hpp"

Server::Server(const char *port, const char *pass) : port(port), password(pass) {
	kq = kqueue();
	if (kq == -1) {
		exit(1);
	}
	initListeningSocket();
	if (addEvent(READ_EVENT, listeningSocket) == IRC_ERROR) {
		exit(1);
	}
	commandHandler = new CommandHandler;
}

Server::~Server() {
	std::cout << "serv destr\n";
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
	std::cout << "accept here\n";
	Client *newClient = new Client(client_fd);
	clients.insert(std::pair<int,Client*>(newClient->getFd(), newClient));
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

int Server::request(int fd) {
	std::string message;
	char buffer[100];

	memset(buffer, 0, sizeof buffer);
	while (!std::strstr(buffer, "\r\n")) {
		if (recv(fd, buffer, 100, 0) < 0) {
			// if (errno != EWOULDBLOCK)
			throw std::runtime_error("Error: socket creation.");
		}
		message.append(buffer);
	}
	commandHandler->handle(clients[fd], message);
	return IRC_OK;
}

int Server::response(int fd) {
	int lenSent;

//	clients[fd]->response();
	std::string repl = clients[fd]->getReply(); // serialize?
	lenSent = send(fd, repl.c_str(), repl.size(), 0);
	return lenSent;
}

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
			request(event.ident);
			addEvent(WRITE_EVENT, eventFd);
		} else if (event.filter == EVFILT_WRITE) {
			response(event.ident);
		} else
			disconnectClient(eventFd);
	}
	return IRC_OK;
}

int Server::run() {
	for (;;) {
		if (processEvents() == IRC_ERROR) {
			std::cout << "err\n";
			break;
		}
//		if (terminate || quit) {
//			shutdown();
//			break;
//		}
	}
	return IRC_OK;
}

void Server::createChannel(std::string name, str::string key, Client *client) {
	Channel *channel = new Channel(name, key);
	channel->setOp(client);
}

Channel *Server::getChannel(str::string name) {
	for (int i = 0; i < channels.size(); ++i) {
		if (channels[i]->getName() == name)
			return channels[i];
	}
	return 0;
}

void Server::shutdown() {
	close(kq);
	disconnectAllClients();
	close(listeningSocket);
}

void Server::disconnectAllClients() {
	std::map<int,Client*>::iterator it = clients.begin();
	std::map<int,Client*>::iterator ite = clients.end();
	for (; it != ite; ++it) {
		delete it->second;
		clients.erase(it);
	}
	clients.clear();
}

void Server::disconnectClient(int fd) {
	delete clients[fd];
	clients.erase(fd);
}
