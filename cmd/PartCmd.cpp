#include "Command.hpp"

#define PART_DEFAULT "no reason specified"

PartCmd::PartCmd(bool auth, Server *server) : Command(auth, server) {}

void PartCmd::execute(Client *client, std::vector <std::string> args) {
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
		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(client->getNickname(), name));
	} else {
		std::string reason = PART_DEFAULT;
		if (args.size() > 1)
			reason = utils::vect_to_string(args, 1);

		//client->addReply(RPL_PART(client->getPrefix(), name, reason));
		channel->broadcast(RPL_PART(client->getPrefix(), name, reason));
		server_->broadcastEvent(client);
		client->leaveChannel(channel);
		logger::info(SSTR("Channel: " << name << " Users: " << channel->getUserNum()));
	}
}
