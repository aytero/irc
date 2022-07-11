#ifndef IRC_SOCKET_HPP
#define IRC_SOCKET_HPP


class Socket {
protected:
	int fd;
public:
	Socket(int fd);
	virtual ~Socket();
};


#endif //IRC_SOCKET_HPP
