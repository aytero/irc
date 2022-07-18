#include "Command.hpp"

JoinCommand::JoinCommand(bool auth, Server *server) : Command(auth, server) {}
//JoinCommand::JoinCommand(const JoinCommand &ref) {}
JoinCommand::~JoinCommand() {}
//JoinCommand &JoinCommand::operator=(const JoinCommand &ref) {}

void JoinCommand::execute(Client *client, std::vector<std::string> args) {
	if (args.empty()) {
		client->setReply(ERR_NEEDMOREPARAMS("JOIN"))
		return;
	}
	if (args[0] == "0") {
		// leave all channels
		return;
	}
	std::string chanName = args[0];
	std::string key = "";
	if (args.size() > 1)
		key = args[1];
	if (server->getChannelNum() > SERVER_MAX_CHANNELS || client->getChannelNum() > CLIENT_MAX_CHANNELS) {
		client->setReply(ERR_TOOMANYCHANNELS());
		return;
	}

	Channel *channel = server->getChannel(chanName);
	if (!channel) {
		channel = server->createChannel(chanName, key, client);
		return;
	}
	// check invite if invite-only
	if (channel->isFull()) {
		client->setReply(ERR_CHANNELISFULL(chanName));
	} else if (channel->checkIfBanned(client->getNickname())) {
		client->setReply(ERR_BANNEDFROMCHAN(chanName));
	} else if (channel->getKey() != key) {
		client->setReply(ERR_BADCHANNELKEY(chanName, key));
	} else {
		client->joinChannel(channel);
//		client->setReply(RPL_TOPIC());
	}
}
