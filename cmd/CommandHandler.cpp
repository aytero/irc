#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {
//	commands["JOIN"] = new JoinCommand();
	commands["PING"] = new PingCommand(true);
//	commands["PONG"] = new PongCommand();
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

void CommandHandler::handle(Server *server, Client *client, std::string &message) {
//	std::stringstream ss;
	std::vector<std::string> args = parse(message);
//	for (ss) {
//		args.push_back();
//	}
//	client->addCommand();
	//if command not in commands -> error
	commands["PING"]->execute(server, client, args);
}
