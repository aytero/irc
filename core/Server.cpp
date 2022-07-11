#include "Server.hpp"

Server::Server(const char *port, const char *pass) : port(port), password(pass) {
	initListeningSocket();
	if (addEvent(READ_EVENT, listeningSocket) == IRC_ERROR) {
		logger::error("failed to configure server socket for reading");
		exit(1);
	}
	logger::info("Server started successfully");
	commandHandler = new CommandHandler;
}

int Server::addEvent(int eventType, int fd) {
	struct kevent event;

	memset(&event, 0, sizeof(event));
	if (eventType == EventType::READ_EVENT)
		EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, 0);
	else
		EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, 0);
	if (kevent(kq, &event, 1, NULL, 0, NULL) == IRC_ERROR)
		return IRC_ERROR;
	return IRC_OK;
}

void Server::initListeningSocket() {
	int opt = 1;

	listeningSocket = ::socket(PF_INET, SOCK_STREAM, 0);
	if (listeningSocket == -1) {
		logger::error("socket creation");
		throw std::runtime_error("Error: socket creation.");
		return;
	}
	if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) == -1) {// size of int?
		logger::error("setsockport");
	}
	address.sin_family = PF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(std::stoi(port));

	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	if (bind(listeningSocket, (struct sockaddr *)&address, sizeof(address))) {
		logger::error("bind");
		throw std::runtime_error("Error bind.");
	}
	if (listen(listeningSocket, 100)) {//serv.max_listen
		logger::error("listen");
		throw std::runtime_error("Error listen.");
	}
}

int Server::acceptConnection(int event_fd) {
	struct sockaddr_in client_addr;
	int addrlen = sizeof client_addr;
	int client_fd;

	if ((client_fd = ::accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0) {
		logger::error("Failed to accept new client");
		return IRC_ERROR;
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	if (addEvent(READ_EVENT, client_fd) == IRC_ERROR) {
		close(client_fd);
		logger::error("Failed to accept new client");
		return IRC_ERROR;
	}
	std::string mes = "Accepted new client with fd " + std::to_string(client_fd);
	logger::info(mes);
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
	while (!std:strstr(buffer, "\r\n")) {
		if (recv(fd, buffer, 100, 0) < 0) {
			// if (errno != EWOULDBLOCK)
			throw std::runtime_error("Error: socket creation.");
		}
		message.append(buffer);
	}
	commandHandler->handle(clients[fd], message)
	return IRC_OK;
}

int response(int fd) {
	int lenSent;

//	clients[fd]->response();
	lenSent = send(fd, clients[fd]->getResponseText(), clients[fd]->getResponseLen(), 0);
	return lenSent;
}

int Server::processEvents() {
	int new_events_num;
	struct kevent eventList[MAX_EVENTS];

	new_events_num = kevent(kq, 0, 0, eventList, MAX_EVENTS, NULL);
		if (new_events_num == IRC_ERROR)
			return -1;

	for (int i = 0; i < new_events_num; ++i) {
		struct kevent &event = eventList[i];
		int eventFd = event.ident;
		if (event.flags & EV_EOF)
			disconnectClient(eventFd);
		else if (eventFd == listeningSocket) {
			acceptConnection(event);
		} else if (event.filter == EVFILT_READ) {
			request(event, clients[eventFd]);
			addEvent(WRITE_EVENT, eventFd);
		} else if (event.filter == EVFILT_WRITE) {
			response(event, clients[eventFd]);
		} else
			disconnectClient(eventFd);
	}
	return IRC_OK;
}

int Server::run() {
	for (;;) {
		if (processEvents() == IRC_ERROR) {
			logger::error("Event processing error");
		}
		if (terminate || quit) {
//			shutdown();
			break;
		}
	}
	return IRC_OK;
}

void Server::shutdown() {
	logger::info("Starting graceful shutdown");
	disconnectAllClients();
	close(listeningSocket);
	closeAllSockets();
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
	std::string mes = std::to_string(fd) + " has disconnected";
	logger::info(mes);
}
