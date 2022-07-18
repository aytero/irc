#include "Command.hpp"

PartCommand::PartCommand(bool auth, Server *server) : Command(auth, server) {}

void PartCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.empty()) {
		client->addReply(ERR_NEEDMOREPARAMS("PART"));
		return;
	}
	Channel *channel;
	std::string name = args[0];
	channel = server->getChannel(name);
	if (!channel) {
		client->addReply(ERR_NOSUCHCHANNEL(name));
	} else if (client->getChannel(name) == 0) {
		client->addReply(ERR_NOTONCHANNEL(name));
	} else {
		client->leaveChannel(channel);
//		repl.append()
	}
}