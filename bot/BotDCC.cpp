#include "BotDCC.hpp"

BotDCC::BotDCC() : stop(false) {
	// connect to server
}

BotDCC::BotDCC(std::string port, std::string pass) : stop(false) {
	// connect to the serv

	int valread;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation");
//		return (-1);
	}

	serv_addr.sin_family = PF_INET;
	serv_addr.sin_port = htons(std::stoi(port));

	if (inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("invalid address");
//		return (-1);
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
		perror("connection failed");
//		return (-1);
	}

	std::string mes = "PASS " + pass + "\r\n";
	send(sock, mes.c_str(), mes.size(), 0);
	mes = "USER dcc_bot * * :DCC file transfer bot\r\n";
	send(sock, mes.c_str(), mes.size(), 0);
	mes = "NICK dcc_bot\r\n";
	send(sock, mes.c_str(), mes.size(), 0);
}

void BotDCC::sendFile(const std::string &filename) {
	std::string fileContent; // what if 0 ? better use char *
	char buf[256];
	memset(buf, 0, 256);
	FILE *fd = fopen(filename.c_str(), "rb");
	if (!fd) {
		logger::error("failed to open file " + filename + " Error: " + strerror(errno));
		return;
	}
	while (!feof(fd)) {
		int bytes_read = fread(&buf, 1, sizeof(buf), fd);
		if (bytes_read < 0)
			break;
		fileContent.append(buf);
	}
	std::cout << fileContent << "\n";
}

void BotDCC::run() {
//	std::string input;
//	getline(std::cin, input);
//	while (input != "QUIT" || input != "exit") {
//		// parse input
//		getline(std::cin, input);
//	}

	int lenRead;
	int bufferLen = 256;
	char buffer[bufferLen];

//	buffer = new char [bufferLen];
//	memset(buffer, 0, bufferLen);

	while (!stop) {
		memset(buffer, 0, bufferLen);
		lenRead = recv(sock, buffer, bufferLen, 0);
		if (lenRead < 1) { // < 0 ?
			logger::error("recv failed/ server is down");
			return;
		}
//		if (lenRead == 0) {
//			logger::info("Connection ended");
//			return;
//		}
		buffer[lenRead] = 0;
		logger::debug(SSTR("lenRead: " << lenRead));
		logger::debug(SSTR("fd: " << sock << " read: " << buffer)); // heap buffer overflow

		std::string str(buffer, lenRead);
		if (str.find("SEND") != std::string::npos) {
			logger::info("Sending a file");
			std::string path = str.substr(str.find("SEND") + 4);
			path.pop_back(); // \n
			logger::info(path);
			sendFile(path);
		}

	}
//	delete buffer;
}

// ./botDCC <port> <password>
int main(int argc, char **argv) {
	if (argc < 3)
		return 0;

	logger::setLogLevel(logger::DEBUG);
	std::string port = argv[1];
	std::string pass = argv[2];
	BotDCC bot(port, pass);
	bot.run();
}