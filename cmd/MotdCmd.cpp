#include "Command.hpp"
#include <fstream>
#include <cstdlib>

MotdCmd::MotdCmd(bool auth, Server *server) : Command(auth, server) {}
//MotdCmd::MotdCmd(const MotdCmd &ref) {}
MotdCmd::~MotdCmd() {}
//MotdCmd &MotdCmd::operator=(const MotdCmd &ref) {}



//  - When responding to the MOTD message and the MOTD file
//    is found, the file is displayed line by line, with
//    each line no longer than 80 characters, using

std::string MotdCmd::getMOTD() {
	std::string motd = "Message of the day:\n";
	std::ifstream file;
	std::string filename = "forest.txt";

	file.open(filename.c_str(), std::ios::in);
	if (!file) {
		logger::error("Error: failed to open MOTD " + filename);
		return "";
	}

	std::string	line;
	while (getline(file, line)) {
		motd.append(line);

		if (!file.eof())
			motd.append("\n");
	}
	return motd;
}

//           RPL_MOTDSTART                   RPL_MOTD
//           RPL_ENDOFMOTD                   ERR_NOMOTD
void MotdCmd::execute(Client *client, std::vector<std::string> args) {
	(void) args;
	client->addReply(server_->getHostname(), RPL_MOTDSTART(client->getNickname(), server_->getHostname()));
	std::string motd = getMOTD();
	if (motd.empty())
		client->addReply(server_->getHostname(), ERR_NOMOTD(client->getNickname()));
	else
		client->addReply(server_->getHostname(), RPL_MOTD(client->getNickname(), motd));
	client->addReply(server_->getHostname(), RPL_ENDOFMOTD(client->getNickname()));
}
