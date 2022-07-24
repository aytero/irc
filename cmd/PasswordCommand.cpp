#include "Command.hpp"

PasswordCommand::PasswordCommand(bool auth, Server *server) : Command(auth, server) {}

void PasswordCommand::execute(Client *client, std::vector <std::string> args) {
	if (client->isRegistered()) {
		client->addReply(server_->getHostname(), ERR_ALREADYREGISTERED(client->getNickname()));
	} else if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("PASS")));
	} else {
		client->setPassword(args[0]);
		client->setState(NICKNAME);
//		client->addReply(server_->getHostname(), ":password set");
	}
}