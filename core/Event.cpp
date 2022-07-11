#include "Event.hpp"

Event::Event() {
	kq = kqueue();
	if (kq == -1) {
		logger::error("Failed to init kqueue");
		exit(1);
	}
}

Event::~Event() {
	close(kq);
}

int Event::addReadEvent(int fd) {//const
	struct kevent event;

	memset(&event, 0, sizeof(event));
	EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, 0); // f: udata ptr to handler
	if (kevent(kq, &event, 1, NULL, 0, NULL) == WS_ERROR)
		return WS_ERROR;
	return WS_OK;
}

int Event::addWriteEvent(int fd) {
	struct kevent event;

	memset(&event, 0, sizeof(event));
	EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, 0);
	if (kevent(kq, &event, 1, NULL, 0, NULL) == WS_ERROR)
		return WS_ERROR;
	return WS_OK;
}

int Event::getNewEvents(struct kevent *eventList) {
	return kevent(kq, 0, 0, eventList, MAX_EVENTS, NULL);
}

Session *Event::acceptConnection(int event_fd) {
	struct sockaddr_in client_addr;
	int addrlen = sizeof client_addr;
	int client_fd;

	if ((client_fd = ::accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0) {
		logger::error("Failed to accept new client");
		return NULL;
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	if (addReadEvent(client_fd) == WS_ERROR) {
		close(client_fd);
		logger::error("Failed to accept new client");
		return NULL;
	}
	std::string mes = "Accepted new client with fd " + std::to_string(client_fd);
	logger::info(mes);
//	sessions.insert(std::pair<int,Session*>(newSession->getFd(), newSession));
	return new Session(client_fd, event_fd);
}

int Event::receiveRequest(const struct kevent &event, Session *session) {
	char *buffer;
	ssize_t lenRead;
	size_t bufferLen = event.data; // get from config of the server
	int event_fd = event.ident;

	buffer = new char [bufferLen];
	lenRead = recv(event_fd, buffer, bufferLen, 0);

	if (lenRead > 0) {
		logger::debug(buffer);
		session->reqParser(buffer, bufferLen);
	}
	delete [] buffer;
	return lenRead;
}

int Event::processRequest(const struct kevent &event, Session *session) {

	if (receiveRequest(event, session) < 1)
		return WS_ERROR;

	int ps = session->getParseStatus()
	switch (ps) {
		case Request::CONTINUE_READING:
//			addReadEvent(event.ident);
			break;
		case Request::FINISH_READING:
			session->processRequest();
			addWriteEvent(event.ident);
			break;
		case Request::PARSE_ERROR:
			break;
//		default: ;
	}
	return WS_OK;
}

int Event::sendResponse(const struct kevent &event, Session *session) {
	int lenSent;

	if (session->getResponseLen() <= event.data) {
		lenSent = send(session->getFd(), session->getResponseText(), session->getResponseLen(), 0);
		session->clearResponse();
		// clearRequest();
	} else {
		lenSent = send(session->getFd(), session->getResponseText(), event.data, 0);
		session->setResponseOffset(session->getResponseOffset() + lenSent);
		if (lenSent > 0)
			addWriteEvent(session->getFd());
	}
	return lenSent;
}