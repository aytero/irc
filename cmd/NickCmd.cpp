#include "Command.hpp"
#include <cctype>

#include <fstream>
#include <cstdlib>
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
	for (int i = 0; i < nickname.size(); ++i) {
		if (!std::isalnum(nickname[i]) && !isSpecial(nickname[i]) && nickname[i] != '-') {
			return false;
		}
	}
	return true;
}

std::string NickCmd::getMOTD() {
    std::string motd = "Message of the day:\n";
    std::ifstream file;
    std::string filename = "forest.txt";

    file.open(filename.c_str(), std::ios::in);
    if (!file)
        logger::error("Error: failed to open MOTD " + filename);

    std::string	line;
    while (getline(file, line)) {
        motd.append(line);

        if (!file.eof())
            motd.append("\n");
    }
    return motd;
}

void NickCmd::welcome(Client *client) {
    const std::string &hostname = server_->getHostname();

    //client->addReply(hostname, RPL_WELCOME(client->getNickname(), client->getPrefix()));
    client->addReply(hostname, RPL_YOURHOST(client->getNickname(), client->getHostname(), std::string("1.1")));
//    client->addReply(hostname, RPL_CREATED(client->getNickname(), server_->getHostname())); // date of server creation
    client->addReply(hostname, RPL_MYINFO(client->getNickname(), server_->getHostname(), std::string("1.1"),
                                          std::string("oOa"), std::string("O")));

//    client->addReply(hostname, RPL_LUSERCLIENT());
//    client->addReply(hostname, RPL_LUSEROP(std::to_string(server_->get)));
//	client->addReply(hostname, RPL_LUSERUNKNOWN()); //
    client->addReply(hostname, RPL_LUSERCHANNELS(client->getNickname(), std::to_string(server_->getChannelNum())));

    client->addReply(hostname, RPL_MOTDSTART(client->getNickname(), server_->getHostname()));
    client->addReply(hostname, RPL_MOTD(client->getNickname(), getMOTD()));
    client->addReply(hostname, RPL_ENDOFMOTD(client->getNickname()));
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
		client->setState(USERNAME);
        client->addReply(server_->getHostname(), RPL_WELCOME(client->getNickname(), client->getPrefix()));
		if (client->getNickname() != "" && client->getRealname() != "") {// username
			client->setState(DONE);
			welcome(client);
		}
//		if (client->getState() != DONE) {
//			client->setState(DONE);
//            welcome(client);
//		}
	}
}
