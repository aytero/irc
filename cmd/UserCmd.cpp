#include "Command.hpp"

UserCmd::UserCmd(bool auth, Server *server) : Command(auth, server) {}

void UserCmd::execute(Client *client, std::vector <std::string> args) {
	if (client->isRegistered()) {
		client->addReply(server_->getHostname(), ERR_ALREADYREGISTERED(client->getNickname()));
	} else if (args.size() < 4) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("USER")));
	} else {
		client->setUsername(args[0]);
		std::string real = utils::vect_to_string(args, 3);
		client->setRealname(real);

		server_->checkConnectionRegistration(client);
	}
}
