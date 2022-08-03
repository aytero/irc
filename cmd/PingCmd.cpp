#include "Command.hpp"

PingCmd::PingCmd(bool auth, Server *server) : Command(auth, server) {}
//PingCmd::PingCmd(const PingCmd &ref) {}
PingCmd::~PingCmd() {}
//PingCmd &PingCmd::operator=(const PingCmd &ref) {}


//	ERR_NOORIGIN()
//	ERR_NOSUCHSERVER()

void PingCmd::execute(Client *client, std::vector<std::string> args) {
	if (args.empty()) {
		client->addReply(server_->getHostname(), "409", ERR_NOORIGIN());
		return;
	}

	if (args.size() == 1) {
		if (args[0] == server_->getHostname()) {
			client->addReply(server_->getHostname(), RPL_PONG(server_->getHostname(), client->getNickname()));
			return;
		}

		//			// todo irssi ping
		std::string &to = args[0];
		Client *target = server_->getClient(to);
		if (!target) {
			logger::debug("No ping target");
			return;
		}
		target->addReply(server_->getHostname(), RPL_PING(client->getNickname()));
		server_->addEvent(WRITE_EVENT, target->getFd());
	}
}
