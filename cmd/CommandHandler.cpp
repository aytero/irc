#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {}

CommandHandler::CommandHandler(Server *server) : server_(server) {
//	commands["JOIN"] = new JoinCommand(true, server_);
//	commands["PART"] = new PartCommand(true, server_);
//	commands["PRIVMSG"] = new PrivMsgCommand(true, server_);
//	commands["PING"] = new PingCommand(true, server_);
	//	commands["PONG"] = new PongCommand();
	commands["PASS"] = new PasswordCommand(false, server_);
	commands["NICK"] = new NickCommand(false, server_);
	commands["USER"] = new UserCommand(false, server_);
}
//CommandHandler::CommandHandler() {}
CommandHandler::~CommandHandler() {
	std::map<std::string, Command*>::iterator it = commands.begin();
	std::map<std::string, Command*>::iterator ite = commands.end();
	for (; it != ite; ++it) {
		delete it->second;
	}
	commands.clear();
}
//CommandHandler &CommandHandler::operator=() {}

std::vector<std::string> CommandHandler::parse(std::string &message) {
	std::vector<std::string> args;
	args.push_back(message);
	return args;
}

//void CommandHandler::handle(Client *client, std::string &message) {
//	std::stringstream ss(message);
//	std::vector<std::string> args;// = parse(message);
//	std::string buf;
//	while (ss >> buf) {
//		args.push_back(buf);
//		std::cout << buf << "\n";
////		client->addReply(buf);
//	}
//	// loop parse + execute
////	for (ss) {
////		args.push_back();
////	}
////	client->addCommand();
//	//if command not in commands -> error
//	commands["PING"]->execute(client, args);
//}

void CommandHandler::handle(Client *client, std::string &message) {
	std::stringstream ss(message);
	std::string separator;

	while (std::getline(ss, separator)) {
		separator = separator.substr(0,
									 separator[separator.size() - 1] == '\r' ? separator.size() - 1 : separator.size());
		std::string cmd = separator.substr(0, separator.find(' '));

		try {
			Command *command = commands.at(cmd);
			std::vector <std::string> args;
			std::string buf;
			std::stringstream ssin(separator.substr(cmd.size(), separator.size()));
			while (ssin >> buf)
				args.push_back(buf);
			if (!client->isRegistered() && command->authRequired()) {
				client->addReply(ERR_NOTREGISTERED());
				return;
			}
			command->execute(client, args);
		} catch (const std::out_of_range &e) {
			client->addReply(ERR_UNKNOWNCOMMAND());
		}
	}
}
