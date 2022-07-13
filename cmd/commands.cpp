#include "commands.hpp"

// JOIN
JoinCommand::JoinCommand(bool auth) : Command(auth) {}
//JoinCommand::JoinCommand(const JoinCommand &ref) {}
JoinCommand::~JoinCommand() {}
//JoinCommand &JoinCommand::operator=(const JoinCommand &ref) {}

void JoinCommand::execute(Client *client, std::vector<std::string> args) {
	client->setReply(args[0]);
}

// PING
PingCommand::PingCommand(bool auth) : Command(auth) {}
//PingCommand::PingCommand(const PingCommand &ref) {}
PingCommand::~PingCommand() {}
//PingCommand &PingCommand::operator=(const PingCommand &ref) {}


void PingCommand::execute(Client *client, std::vector<std::string> args) {
//	ERR_NOORIGIN()
//	ERR_NOSUCHSERVER()
//	if (client->checkLastActivity() == 0) {
//		client->deleteFromAllChannels();
//		disconnect()
//	}
	// or server->getPrefix()
//	std::string repl = ":" + client->getPrefix() + " PING :" + args.at(0);
	std::string repl = ":" + client->getNickname() + " PING :" + args.at(0);
	client->setReply(repl);
}
