#ifndef IRC_EVENT_HPP
# define IRC_EVENT_HPP

# define MAX_EVENTS 128

class Event {
	int kq;

public:
	Event();
	Event(const EventManager &ref);
	~Event();
	Event &operator=(const Event *ref);

	int addReadEvent(int fd);
	int addWriteEvent(int fd);
	int getNewEvents(struct kevent *eventList);
	Session *acceptConnection(int eventFd);
	int receiveRequest(const struct kevent &event, Session *session);
	int processRequest(const struct kevent &event, Session *session);
	int sendResponse(const struct kevent &event, Session *session);
};

#endif //IRC_EVENT_HPP
