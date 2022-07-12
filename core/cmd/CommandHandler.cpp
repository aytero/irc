#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {
//	commands["JOIN"] = new JoinCommand();
	commands["PING"] = new PingCommand(true);
//	commands["PONG"] = new PongCommand();
}
//CommandHandler::CommandHandler() {}
CommandHandler::~CommandHandler() {
}
//CommandHandler &CommandHandler::operator=() {}

std::vector<std::string> CommandHandler::parse(std::string &message) {
	std::vector<std::string> args;
	args.push_back("PING");
	return args;
}

void CommandHandler::handle(Client *client, std::string &message) {
//	std::stringstream ss;
	std::vector<std::string> args = parse(message);
//	for (ss) {
//		args.push_back();
//	}
//	client->addCommand();
	commands["PING"]->execute(client, args);
}
