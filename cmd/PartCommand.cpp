#include "Command.hpp"

#define PART_DEFAULT "no reason specified"

PartCommand::PartCommand(bool auth, Server *server) : Command(auth, server) {}

void PartCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("PART")));
		return;
	}

	Channel *channel;
	std::string name = args[0];
	channel = server_->getChannel(name);

	if (!channel) {
//		if no users delete chan?
		client->addReply(server_->getHostname(), ERR_NOSUCHCHANNEL(client->getNickname(), name));
	} else if (client->getChannel(name) == 0) {
		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(name));
	} else {
//		if no users delete chan?

		std::string reason;
		if (args.size() > 1) {
			for (int i = 1; i < args.size(); ++i)
				reason.append(args[i] + " "); // SP after last word _._
		} else
			reason = PART_DEFAULT;
		//client->addReply(RPL_PART(client->getPrefix(), name, reason));
		channel->broadcast(RPL_PART(client->getPrefix(), name, reason));
		server_->broadcastEvent(client);
		client->leaveChannel(channel);
		logger::info(SSTR("Channel: " << name << " Users: " << channel->getUserNum()));
		if (channel->getUserNum() == 0)
			server_->deleteChannel(name);
	}
}
