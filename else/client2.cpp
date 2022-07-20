#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <sstream>
#include <string>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int	main( int argc, char **argv ) {
	int		sock = 0;
	int		valread;
	struct sockaddr_in	serv_addr;
	//								 1718 :21		len=62
//	char	*mes = "GET /f HTTP/1.1\r\nHost: localhost:8000\r\nAccept-Language: fr\r\n\r\n";
	char	buffer[1024] = {0};

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation");
		return (-1);
	}

	serv_addr.sin_family = PF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("invalid address");
		return (-1);
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
		perror("connection failed");
		return (-1);
	}

	sleep(3);
	std::string mes = "PASS www\r\n";
	send(sock, mes.c_str(), mes.size(), 0);

//	valread = read(sock, buffer, 1024);
//	std::cout << buffer << "\n";

	mes = "NICK ton\r\n";
	send(sock, mes.c_str(), mes.size(), 0);

//	valread = read(sock, buffer, 1024);
//	std::cout << buffer << "\n";

	mes = "USER pony 0 * toh\r\n";
	send(sock, mes.c_str(), mes.size(), 0);

	memset(buffer, 0, sizeof buffer);
	valread = read(sock, buffer, 1024);
	std::cout << buffer << "\n";

	sleep(5);
//	mes = "PRIVMSG ayo emm u here?\r\n";
//	send(sock, mes.c_str(), mes.size(), 0);

//	memset(buffer, 0, sizeof buffer);
//	valread = read(sock, buffer, 1024);
//	std::cout << buffer << "\n";

	mes = "JOIN #hbd\r\n";
	send(sock, mes.c_str(), mes.size(), 0);
	memset(buffer, 0, sizeof buffer);
	valread = read(sock, buffer, 1024);
	std::cout << buffer << "\n";

	memset(buffer, 0, sizeof buffer);
	valread = read(sock, buffer, 1024);
	std::cout << buffer << "\n";
	return (0);
}
