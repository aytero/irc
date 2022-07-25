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
		// add oper SIGN to prefix
//			server then informs the rest of the network
//		of the new operator by issuing a "MODE +o" for the clients nickname
		client->setOpername(args[0]);
		server_->addOper(args[0]);
		client->addReply(server_->getHostname(), RPL_YOUREOPER());
		// MODE +o
		client->addReply(server_->getHostname(), RPL_MODE(client->getNickname(), client->getModeStr()));
//		client->addReply(server_->getHostname(), RPL_UMODEIS(client->getModeStr()));
	}
}
