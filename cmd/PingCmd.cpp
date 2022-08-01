#include "Command.hpp"

PingCmd::PingCmd(bool auth, Server *server) : Command(auth, server) {}
//PingCmd::PingCmd(const PingCmd &ref) {}
PingCmd::~PingCmd() {}
//PingCmd &PingCmd::operator=(const PingCmd &ref) {}


void PingCmd::execute(Client *client, std::vector<std::string> args) {
	//	ERR_NOORIGIN()
	//	ERR_NOSUCHSERVER()
	//	if (client->checkLastActivity() == 0) {
	//		client->deleteFromAllChannels();
	//		disconnect()
	//	}
	// or server->getPrefix()
	//	std::string repl = ":" + client->getPrefix() + " PING :" + args.at(0);

	if (args.empty())
		return;
	std::string to;
	std::string from;
	if (args.size() == 1) {
		from = server_->getHostname();
		to = args[0];
	} else {
		from = args[0];
		to = args[1];
	}
//	client->addReply("PING :serv+prefix");
	Client *target = server_->getClient(to);
	if (!target) {
		logger::error("no ping target");
		return;
	}
	target->addReply(server_->getHostname(), RPL_PING(from));
//	server_->addEvent(WRITE_EVENT, target->getFd());
}
