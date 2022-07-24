#include "Command.hpp"

//ERR_NEEDMOREPARAMS              RPL_YOUREOPER
//ERR_NOOPERHOST                  ERR_PASSWDMISMATCH

OperCommand::OperCommand(bool auth, Server *server) : Command(auth, server) {}

void OperCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.size() < 2) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("OPER")));
//	} else if (server_->getMode('O') || client->getHostname() != server_->allowedOperHost()) {
//		client->addReply(server_->getHostname(), ERR_NOOPERHOST());
	}
	else if (args[1] != server_->getOperPassword()) {
		client->addReply(server_->getHostname(), ERR_PASSWDMISMATCH());
	}
	else {
		client->switchMode('o', true);
		client->setOpername(args[0]);
		client->addReply(server_->getHostname(), RPL_YOUREOPER());
		client->addReply(server_->getHostname(), RPL_UMODEIS(client->getModeStr()));
	}
}
