#include "commands.hpp"

void PartCommand::execute(Server *server, Client *client, std::vector <std::string> args) {
	if (args.empty()) {
		client->setReply(ERR_NEEDMOREPARAMS("PART"));
		return;
	}
	std::string repl = "";
	Channel *channel;
	for (int i = 0; i < args.size(); ++i) {
		channel = server->getChannel(args[i]);
		if (!channel) {
			repl.append(ERR_NOSUCHCHANNEL(args[i]));
		} else if (client->getChannel(args[i]) == 0) {
			repl.append(ERR_NOTONCHANNEL(args[i]));
		} else {
			client->leaveChannel(channel);
//			repl.append()
		}
	}
	client->setReply(repl);
}