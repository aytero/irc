#include "Command.hpp"

UserCommand::UserCommand(bool auth, Server *server) : Command(auth, server) {}

void UserCommand::execute(Client *client, std::vector <std::string> args) {
	if (client->isRegistered()) {
		client->addReply(ERR_ALREADYREGISTERED(client->getNickname()));
	} else if (args.size() < 4) {
		client->addReply(ERR_NEEDMOREPARAMS(std::string("USER")));
	} else {
		client->setUsername(args[0]);
		client->setRealname(args[3]);
		client->setState(DONE);
		client->addReply(server_->getHostname(), RPL_WELCOME(client->getPrefix()));
//		client->welcome();
	}
}