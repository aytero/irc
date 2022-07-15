#include "commands.hpp"

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

void NamesCommand::execute(Client *client, std::vector<std::string> args) {
	std::vector<Channel*>::iterator it = client->getChannels().begin();
	std::vector<Channel*>::iterator ite = client->getChannels().end();
	std::string repl;
	for (; it != ite; ++it) {
		repl.append("dd");
	}
	client->setReply(repl);
}
