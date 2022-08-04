#include "CampfireBot.hpp"

CampfireBot::CampfireBot() : stop(false) {
	// connect to server
}

CampfireBot::CampfireBot(std::string port, std::string pass) : stop(false), channel_("#campfire") {

	// connect to the serv
	struct sockaddr_in serv_addr;

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		logger::error("socket creation");
		throw std::runtime_error("socket creation.");
	}

	serv_addr.sin_family = PF_INET;
	serv_addr.sin_port = htons(std::stoi(port));

	if (inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		logger::error("invalid address");
		throw std::runtime_error("invalid address");
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
		logger::error("connection failed");
		throw std::runtime_error("connection failed");
	}

	sendReply("PASS " + pass);
	sendReply("USER bot * * :Campfire bot");
	sendReply("NICK bot");
	sendReply("JOIN " + channel_);
	sendReply("TOPIC " + channel_ + " :The bot's campfire");


//	priv_ = "PRIVMSG " + channel_;
}

void CampfireBot::sendReply(std::string mes) {
	logger::debug("send reply: " + mes);
	mes.append("\r\n");
	send(sock, mes.c_str(), mes.size(), 0);
}


void CampfireBot::initGuessNumber() {
	
	srand(std::time(0));
	num_ = rand() % 100 + 1;

	sendReply("PRIVMSG " + channel_ + "Welcome to the Guess number game!");
	sendReply("PRIVMSG " + channel_ + "You can use FINISH to stop the game.");
	sendReply("PRIVMSG " + channel_ + "Enter a guess between 1 and 100: ");
}

void CampfireBot::checkGuess(int guess) {

	std::string mes = "";
	++tries_;
	if (guess > num_) {
		mes = "Too high! Try again, you have " + std::to_string(MAX_TRIES - tries_) + " left";
	} else if (guess < num_) {
		mes = "Too low! Try again, you have " + std::to_string(MAX_TRIES - tries_) + " left";
	} else {
		mes = "Correct! You got it in " + std::to_string(tries_) + " tries!";
		finishGame = true;
	}

	if (tries_ == MAX_TRIES) {
		mes = "You are out of tries! :(   The number was " + std::to_string(num_);
		finishGame = true;
	}
	sendReply("PRIVMSG " + channel_ + " " + mes);
}

void CampfireBot::gameCmd() {
	if (state_ != GAME) {
		state_ = GAME;
		tries_ = 0;
		initGuessNumber();
		//sendReply("PRIVMSG " + channel_ + " ");
		// game
	// tries left

	} else {
		sendReply("PRIVMSG " + channel_ + " you are already playing");
	}
}
void CampfireBot::finishCmd() {
	if (state_ == GAME) {
		sendReply("PRIVMSG " + channel_ + " :finishing the guessing game");
		state_ = RUN; // idle
	} else {
		sendReply("PRIVMSG " + channel_ + " : there is nothing to finish");
	}
}

void CampfireBot::handle(std::string message) {


	// todo cut prefix, privmsg, etc and leave only message

	// prefix
	//logger::warn(message + " message!");
	message.erase(0, message.find(' ') + 1);
	//logger::warn(message);

	std::string serv_cmd = message.substr(0, message.find(' '));
	message.erase(0, message.find(' ') + 1);

	if (serv_cmd == "PING") {
		sendReply("PONG " + message.substr(0, message.find(' ')));
		return;
	}
	
	std::string chan = message.substr(0, message.find(' '));
	message.erase(0, message.find(' ') + 2);

	if ((serv_cmd != "PRIVMSG") || chan != channel_) {
		logger::warn(serv_cmd);
		return ;
	}
	

	std::stringstream ss(message);
	std::string separator;

	while (std::getline(ss, separator)) {
		separator = separator.substr(0, separator[separator.size() - 1] == '\r' ? separator.size() - 1 : separator.size());

		std::string cmd  = separator.substr(0, separator.find(" :"));
		//std::string cmd  = separator.substr(0, separator.find(' '));

		std::vector<std::string> args;
		std::string buf;
		std::stringstream ssin(separator.substr(cmd.size(), separator.size()));

		// todo doesnt work with multiple args
		// ex: JOIN #campfire, #bonfire
		while (ssin >> buf) {
			/// undefined ?
			if (buf[0] == ':')//->++buf
				buf = buf.substr(1);
			args.push_back(buf);
		}

		for (unsigned int i = 0; i < args.size(); ++i)
			logger::debug(SSTR("arg[" << i << "]: " <<args[i]));

		if (cmd == "GAME")
			gameCmd();
		if (cmd == "FINISH") {
			finishCmd();
		}
		if (cmd == "PING") {
			sendReply("PONG " + args[1]);
		}

//		if (cmd == "booo")
//			sendReply("PRIVMSG " + channel_ + " baaa! /ghost/");
		//command->execute(client, args);
	}
}

void CampfireBot::run() {
	int lenRead;
	int bufferLen = 256;
	char buffer[bufferLen];

//	buffer = new char [bufferLen];
//	memset(buffer, 0, bufferLen);

	logger::info("loop");

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

		//std::vector<std::string> args = parse(buffer);
		handle(buffer);
		/*
		if (state_ == GAME) {
			try {
				int guess = stoi(args[0]);
				checkGuess(guess);
				if (finishGame)
					state_ = RUN;
			} catch (std::invalid_argument const& ex) {
				logger::info("stoi exception");
			}
		}
		*/
	}
//	delete buffer;
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cout << "Usage: ./app <port> <server password>" << std::endl;
		return 0;
	}

	logger::setLogLevel(logger::DEBUG);

	std::string port = argv[1];
	std::string pass = argv[2];

	CampfireBot bot(port, pass);
	bot.run();
}
