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
	int ret = send(sock, mes.c_str(), mes.size(), 0);
	if (ret <= 0) {
		logger::error("send failure");
	}
}


void CampfireBot::initGuessNumber() {

	state_ = GAME;
	tries_ = 0;
	finishGame = false;
	srand(std::time(0));
	num_ = rand() % 100 + 1;

	sendReply("PRIVMSG " + channel_ + " "B_ORG"Welcome to the Guess number game!"RESET_B);
	sendReply("PRIVMSG " + channel_ + " "B_ORG"You can use !finish to stop the game"RESET_B);
	sendReply("PRIVMSG " + channel_ + " "B_ORG"Enter a guess between 1 and 100: "RESET_B);
}

void CampfireBot::checkGuess(int guess) {
	std::string mes;

	++tries_;
	if (guess > num_) {
		mes = " "YEL"Too high! Try again, you have " + std::to_string(MAX_TRIES - tries_) + " tries left"RESET_B;
	} else if (guess < num_) {
		mes = " "YEL"Too low! Try again, you have " + std::to_string(MAX_TRIES - tries_) + " tries left"RESET_B;
	} else {
		mes = " "B_ORG"Correct! You got it in " + std::to_string(tries_) + (tries_ == 1 ? " try" : " tries!") + RESET_B;
		finishGame = true;
	}

	if (!finishGame && tries_ == MAX_TRIES && guess != num_) {
		mes = " "B_ORG"You are out of tries! The number was " + std::to_string(num_) + RESET_B;
		finishGame = true;
	}
	sendReply("PRIVMSG " + channel_ + " " + mes);
}

void CampfireBot::helpCmd() {

	sendReply("PRIVMSG " + channel_ + " "ORG"I'm a CampfireBot. Come sit at my warm place!"RESET_B);
	sendReply("PRIVMSG " + channel_ + " "ORG"Here are some commands I can do:"RESET_B);
	sendReply("PRIVMSG " + channel_ + " "ORG"!help - to get help"RESET_B);
	sendReply("PRIVMSG " + channel_ + " "ORG"!game - to play some number guessing"RESET_B);
	sendReply("PRIVMSG " + channel_ + " "ORG"!finish - to finish the game"RESET_B);
}

void CampfireBot::gameCmd() {
	if (state_ != GAME) {
		initGuessNumber();
	} else {
		sendReply("PRIVMSG " + channel_ + " "B_ORG":you are already playing"RESET_B);
	}
}

void CampfireBot::finishCmd() {
	if (state_ == GAME) {
		sendReply("PRIVMSG " + channel_ + " "B_ORG":finishing the guessing game"RESET_B);
		state_ = RUN; // idle
	} else {
		sendReply("PRIVMSG " + channel_ + " "B_ORG":there is no game to finish"RESET_B);
	}
}

void CampfireBot::handle(std::string message) {

	Message mes(message);

	// todo: help !help
	// cmd also may parse as numeric
	if (mes.getServCommand() == "PING") {
		if (!mes.getArgs().empty())
			sendReply("PONG " + mes.getArgs()[0]);
		return;
	}

	logger::info(SSTR("Parsed serv command: " << mes.getServCommand()));
	if (mes.getServCommand() == "PRIVMSG" && mes.getChannel() == channel_) {
		std::string &cmd = mes.getArgs()[0];
		if (cmd == "GAME" || cmd == "!game")
			gameCmd();
		else if (cmd == "FINISH" || cmd == "!finish") {
			finishCmd();
		} else if (cmd == "HELP" || cmd == "!help") {
			helpCmd();
		} else if(cmd[0] == '!' && (cmd != "!help" && cmd != "!game" && cmd != "!finish")) {
			sendReply("PRIVMSG " + channel_ + " Unknown bot command!");
		} else if (state_ == GAME && !mes.getArgs().empty()) {
			try {
				int guess = stoi(mes.getArgs()[0]);
				checkGuess(guess);
				if (finishGame)
					state_ = RUN;
			} catch (std::invalid_argument const &ex) {
				logger::info("stoi exception");
			}
		}
//		} else {
//			sendReply("PRIVMSG " + channel_ + "Unknown command")
//		}
		//		if (cmd == "booo")
		//			sendReply("PRIVMSG " + channel_ + " baaa! /ghost/");
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
			logger::error("recv failed or server is down");
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
