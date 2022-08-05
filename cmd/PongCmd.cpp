#include "Command.hpp"

PongCmd::PongCmd(bool auth, Server *server) : Command(auth, server) {}
//PongCmd::PongCmd(const PongCmd &ref) {}
PongCmd::~PongCmd() {}
//PongCmd &PongCmd::operator=(const PongCmd &ref) {}


//	ERR_NOORIGIN()
//	ERR_NOSUCHSERVER()

void PongCmd::execute(Client *client, std::vector<std::string> args) {

	if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NOORIGIN());
		return;
	}
	Client *targ = server_->getClient(args[0]);
	if (targ) {
		targ->addReply(client->getPrefix(), RPL_PONG(client->getNickname(), args[0]));
		server_->addEvent(WRITE_EVENT, targ->getFd());
		return;
	}

	if (args[0] != server_->getHostname()) {
		client->addReply(server_->getHostname(), "402", ERR_NOSUCHSERVER(server_->getHostname()));
		return;
	}
	logger::info(SSTR(client->getPrefix() << " PONG"));
	client->statePing(false);
}
