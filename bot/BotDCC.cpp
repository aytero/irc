#include "CampfireBot.hpp"

CampfireBot::CampfireBot() : stop(false) {
	// connect to server
}

CampfireBot::CampfireBot(std::string port, std::string pass) : stop(false) {
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


void CampfireBot::receiveFile(const std::string &filename, unsigned long fileSize) {
	int lenRead;
	int bufLen = 256;
	char buf[bufLen];

	// truncate ? 
	// create
	FILE *pFile = fopen(filename.c_str(), "wb");
	unsigned int sizeCheck = 0;


	// new socket connection?
	while (sizeCheck < fileSize) {
		memset(buf, 0, 256);
		lenRead = recv(sock, buf, 256, 0);
		if (lenRead < 1) { // < 0 ?
			logger::error("recv failed while receiving a file");
			return;
		}
		//fwrite / append
		fwrite(buf, sizeof(char), sizeof(buf), pFile);
		sizeCheck += lenRead;
	}
	fclose(pFile);
}

void CampfireBot::sendFile(const std::string &filename) {
	int bufLen = 256;
	char buf[bufLen];
	std::vector<char> bytes; // BYTE

	FILE *fd = fopen(filename.c_str(), "rb");
	if (!fd) {
		logger::error("failed to open file " + filename + " Error: " + strerror(errno));
		return;
	}

	int bytes_read;

	while (1) {
		memset(buf, 0, bufLen);

		bytes_read = fread(&buf, 1, sizeof(buf), fd);
		logger::debug(SSTR("bytes read: " << bytes_read));

		if (bytes_read < 0) {
			logger::error("fread error");
			return;
		}

		bytes.insert(bytes.end(), buf, buf + bytes_read);

		if (feof(fd)) {
			logger::info("eof");
			break;
		}

		if (ferror(fd))
			logger::error(SSTR("Error Writing to file " << strerror(errno)));
	}

	//std::vector<char>::iterator it = bytes.begin();
	//std::vector<char>::iterator ite = bytes.end();
	char *b = new char[bytes.size()];
	memset(b, 0, bytes.size());
	for (unsigned long i = 0; i < bytes.size(); ++i) {
		b[i] = bytes[i];
	}
	logger::debug(b);

	// set connection
	// send

	fclose(fd);
	delete[] b;
}

void CampfireBot::run() {
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
		if (str.find("GAME") != std::string::npos) {
			// game state
			// finish game
			// tries left
		}
		if (str.find("SEND") != std::string::npos) {
			logger::info("Sending a file");
			//std::string path = str.substr(str.find("SEND") + 4);
			std::string path = "./else/test.txt";
			//if (path.empty() || path == "")
			//path.pop_back(); // \n
			logger::info(path);
			sendFile(path);
			logger::debug("after sending");
			//receiveFile(path, 33);
		}
		if (str.find("GET") != std::string::npos) {
			logger::info("GET file");
			std::string filename = "t.txt";
			unsigned long fileSize = 42;
			receiveFile(filename, fileSize);
			logger::info("GET file end");
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
	CampfireBot bot(port, pass);
	bot.run();
}
