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
	target->addReply(RPL_PING(from));
	server_->addEvent(WRITE_EVENT, target->getFd());
}
