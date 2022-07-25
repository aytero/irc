#include "Command.hpp"
#include "../core/Channel.hpp"
//#include "../core/Server.hpp"

JoinCommand::JoinCommand(bool auth, Server *server) : Command(auth, server) {}
//JoinCommand::JoinCommand(const JoinCommand &ref) {}
JoinCommand::~JoinCommand() {}
//JoinCommand &JoinCommand::operator=(const JoinCommand &ref) {}

void JoinCommand::execute(Client *client, std::vector<std::string> args) {
	if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("JOIN")));
		return;
	}
	if (args[0] == "0") {
//		std::vector<Channel*> chans = client->getChannels();
		// leave all channels
//		for (int i = 0; i < chans.size(); ++i) {
			// PART in a loop
//		}
		return;
	}
	std::string chanName = args[0];
	std::string key = "";
	if (args.size() > 1)
		key = args[1];
	if (server_->getChannelNum() > SERVER_MAX_CHANNELS || client->getChannelNum() > USER_MAX_CHANNELS) {
		client->addReply(server_->getHostname(), ERR_TOOMANYCHANNELS(chanName));
		return;
	}

	Channel *channel = server_->getChannel(chanName);
//	logger::debug(SSTR(chanName << " channel name (JOIN)"));
	if (!channel) {
		channel = server_->createChannel(chanName, key, client);
	}
	// check invite if invite-only
	if (channel->isFull()) {
		client->addReply(server_->getHostname(), ERR_CHANNELISFULL(chanName));
	} else if (channel->checkIfBanned(client->getNickname())) {
		client->addReply(server_->getHostname(), ERR_BANNEDFROMCHAN(chanName));
	} else if (channel->getKey() != key) {
		client->addReply(server_->getHostname(), ERR_BADCHANNELKEY(chanName, key));
	} else {
		client->joinChannel(channel);
		channel->broadcast(RPL_JOIN(client->getPrefix(), chanName));
		server_->broadcastEvent();
		std::string &topic = channel->getTopic();
		if (topic == "")
			client->addReply(RPL_NOTOPIC(chanName));
		else
			client->addReply(RPL_TOPIC(chanName, topic));

	}
}
