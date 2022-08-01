#include "Command.hpp"

PongCmd::PongCmd(bool auth, Server *server) : Command(auth, server) {}
//PongCmd::PongCmd(const PongCmd &ref) {}
PongCmd::~PongCmd() {}
//PongCmd &PongCmd::operator=(const PongCmd &ref) {}


void PongCmd::execute(Client *client, std::vector<std::string> args) {
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
