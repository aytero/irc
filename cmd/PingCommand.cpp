#include "Command.hpp"

PingCommand::PingCommand(bool auth, Server *server) : Command(auth, server) {}
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
