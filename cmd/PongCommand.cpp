#include "Command.hpp"

PongCommand::PongCommand(bool auth, Server *server) : Command(auth, server) {}
//PongCommand::PongCommand(const PongCommand &ref) {}
PongCommand::~PongCommand() {}
//PongCommand &PongCommand::operator=(const PongCommand &ref) {}


void PongCommand::execute(Client *client, std::vector<std::string> args) {
	//	ERR_NOORIGIN()
	//	ERR_NOSUCHSERVER()
	//	if (client->checkLastActivity() == 0) {
	//		client->deleteFromAllChannels();
	//		disconnect()
	//	}
	// or server->getPrefix()
	//	std::string repl = ":" + client->getPrefix() + " PING :" + args.at(0);

	if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NOORIGIN());
		return;
	}
	std::string &from = args[0];
	server_->acceptPong(from);
}
