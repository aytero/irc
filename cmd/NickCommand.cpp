#include "Command.hpp"
//#include "../core/Server.hpp"
#include <cctype>

NickCommand::NickCommand(bool auth, Server *server) : Command(auth, server) {}

//ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//ERR_UNAVAILRESOURCE             ERR_RESTRICTED

// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
//  letter     =  %x41-5A / %x61-7A       ; A-Z / a-z
//  digit      =  %x30-39                 ; 0-9
//  special    =  %x5B-60 / %x7B-7D       ; "[", "]", "\", "`", "_", "^", "{", "|", "}"

bool isSpecial(int c) {
	if ((c >= 91 && c <= 96) || (c >= 123 && c <= 125))
		return true;
	return false;
}

bool NickCommand::validate(std::string nickname) {
	if (nickname.empty() || nickname.size() > 9)
		return false;
	for (int i = 0; i < nickname.size(); ++i) {
		if (!std::isalnum(nickname[i]) && !isSpecial(nickname[i]) && nickname[i] != '-') {
			return false;
		}
	}
	return true;
}

void NickCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.empty() || args[0] == "") {
		client->addReply(ERR_NONICKNAMEGIVEN());
		return;
	}
	std::string nick = args[0];
	if (server_->getClient(nick)) {// use lowercasing to check?
		client->addReply(ERR_NICKNAMEINUSE(nick));
	} else if (!validate(nick)) {
		client->addReply(ERR_ERRONEUSNICKNAME(nick));
	} else {
		client->setNickname(nick);
		if (client->getState() != DONE) {
			client->setState(USERNAME);
//			client->addReply(server_->getHostname(), nick + " :nickname set");
		}
		client->welcome();
	}
}