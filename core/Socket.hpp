#ifndef IRC_SOCKET_HPP
# define IRC_SOCKET_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>

class Socket {
protected:
	int fd;
public:
	Socket(int fd);
	virtual ~Socket();
	int getFd();
};


#endif //IRC_SOCKET_HPP
