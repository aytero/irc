#include "Command.hpp"

PassCmd::PassCmd(bool auth, Server *server) : Command(auth, server) {}

void PassCmd::execute(Client *client, std::vector <std::string> args) {
	if (client->isRegistered()) {
		client->addReply(server_->getHostname(), ERR_ALREADYREGISTERED(client->getNickname()));
	} else if (args.empty()) {
        client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("PASS")));
    } else if (args[0] != server_->getPassword()){
        client->addReply(server_->getHostname(), ERR_PASSWDMISMATCH());
	} else {
		client->setPassword(args[0]);
		server_->checkConnectionRegistration(client);
	}
}