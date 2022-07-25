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
