#include "Command.hpp"
//#include "../core/Server.hpp"

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
		client->addReply(server_->getHostname(), "001", RPL_WELCOME(client->getPrefix()));
//		client->addReply(server_->getHostname(), RPL_YOURHOST(client->getHostname(), "1.1"))
//		client->welcome();
	}
}

//RPL_WELCOME "Welcome to the Internet Relay Network <nick>!<user>@<host>"
//002    RPL_YOURHOST "Your host is <servername>, running version <ver>"
//004    RPL_MYINFO "<servername> <version> <available user modes> <available channel modes>"
