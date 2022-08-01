#include "Command.hpp"

#include <fstream>
#include <cstdlib>

UserCmd::UserCmd(bool auth, Server *server) : Command(auth, server) {}

//  - When responding to the MOTD message and the MOTD file
//    is found, the file is displayed line by line, with
//    each line no longer than 80 characters, using

std::string UserCmd::getMOTD() {
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

void UserCmd::welcome(Client *client) {
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

// todo change order to PASS -> USER -> NICK
void UserCmd::execute(Client *client, std::vector <std::string> args) {
	if (client->isRegistered()) {
		client->addReply(server_->getHostname(), ERR_ALREADYREGISTERED(client->getNickname()));
	} else if (args.size() < 4) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("USER")));
	} else {
		client->setUsername(args[0]);
		std::string real = utils::vect_to_string(args, 3);
		client->setRealname(real);
//		client->setState(NICKNAME);

		client->addReply(server_->getHostname(), RPL_WELCOME(client->getNickname(), client->getPrefix()));
//		if (client->getState() != DONE) {
		if (client->getNickname() != "" && client->getRealname() != "") {
			client->setState(DONE);
			welcome(client);
		}
	}
}
