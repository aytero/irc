#include "Command.hpp"
//#include "../core/Server.hpp"
#include <fstream>
#include <cstdlib>
//✩‧₊*:・text ･:*₊‧✩
//+
//+++
//+x ++x+
//xxx +
UserCommand::UserCommand(bool auth, Server *server) : Command(auth, server) {}

//375    RPL_MOTDSTART
//       ":- <server> Message of the day - "
//372    RPL_MOTD
//       ":- <text>"
//376    RPL_ENDOFMOTD
//       ":End of MOTD command"
//
//  - When responding to the MOTD message and the MOTD file
//    is found, the file is displayed line by line, with
//    each line no longer than 80 characters, using
//
//
//
//
//Kalt                         Informational                     [Page 48]
//
//RFC 2812          Internet Relay Chat: Client Protocol        April 2000
//
//
//           RPL_MOTD format replies.  These MUST be surrounded
//           by a RPL_MOTDSTART (before the RPL_MOTDs) and an
//           RPL_ENDOFMOTD (after).
std::string UserCommand::getMOTD() {
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

// todo change order to PASS -> USER -> NICK
void UserCommand::execute(Client *client, std::vector <std::string> args) {
	if (client->isRegistered()) {
		client->addReply(server_->getHostname(), ERR_ALREADYREGISTERED(client->getNickname()));
	} else if (args.size() < 4) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("USER")));
	} else {
		client->setUsername(args[0]);
		std::string real = "";
		for (int i = 3; i < args.size(); ++i)
			real.append(args[i] + " "); // SP after last word -.-
		client->setRealname(real);
		client->setState(DONE);
		client->addReply(server_->getHostname(), RPL_WELCOME(client->getNickname(), client->getPrefix()));
		client->addReply(server_->getHostname(), getMOTD());
//		client->addReply(server_->getHostname(), RPL_YOURHOST(client->getHostname(), "1.1"))
//		client->welcome();
	}
}
