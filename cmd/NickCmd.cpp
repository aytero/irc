#include "Command.hpp"
#include <cctype>

//✩‧₊*:・text ･:*₊‧✩
//+
//+++
//+x ++x+
//xxx +

NickCmd::NickCmd(bool auth, Server *server) : Command(auth, server) {}

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

bool NickCmd::validate(std::string nickname) {
	if (nickname.empty() || nickname.size() > 9)
		return false;
	for (unsigned int i = 0; i < nickname.size(); ++i) {
		if (!std::isalnum(nickname[i]) && !isSpecial(nickname[i]) && nickname[i] != '-') {
			return false;
		}
	}
	return true;
}

void NickCmd::execute(Client *client, std::vector <std::string> args) {
	if (args.empty() || args[0] == "") {
		client->addReply(server_->getHostname(), ERR_NONICKNAMEGIVEN());
		return;
	}
	std::string &nick = args[0];
	if (server_->isBanned(nick)) {
		client->addReply(server_->getHostname(), "ERR this nickname is banned!");
		client->quit();
	} else if (server_->getClient(nick)) {// use lowercasing to check?
		client->addReply(server_->getHostname(), ERR_NICKNAMEINUSE(nick));
	} else if (!validate(nick)) {
		client->addReply(server_->getHostname(), ERR_ERRONEUSNICKNAME(nick));
	} else {
		client->setNickname(nick);

		if (client->getState() == DONE)
        	client->addReply(server_->getHostname(), RPL_WELCOME(client->getNickname(), client->getPrefix()));
		else
			server_->checkConnectionRegistration(client);
	}
}
