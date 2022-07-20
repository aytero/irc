#include "Command.hpp"
#include "../core/Channel.hpp"
//#include "../core/Server.hpp"

JoinCommand::JoinCommand(bool auth, Server *server) : Command(auth, server) {}
//JoinCommand::JoinCommand(const JoinCommand &ref) {}
JoinCommand::~JoinCommand() {}
//JoinCommand &JoinCommand::operator=(const JoinCommand &ref) {}

void JoinCommand::execute(Client *client, std::vector<std::string> args) {
	if (args.empty()) {
		client->addReply(ERR_NEEDMOREPARAMS(std::string("JOIN")));
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
	if (server_->getChannelNum() > SERVER_MAX_CHANNELS || client->getChannelNum() > USER_MAX_CHANNELS) {
		client->addReply(server_->getHostname(), "405", ERR_TOOMANYCHANNELS(chanName));
		return;
	}

	Channel *channel = server_->getChannel(chanName);
//	logger::debug(SSTR(chanName << " channel name (JOIN)"));
	if (!channel) {
		channel = server_->createChannel(chanName, key, client);
	}
	// check invite if invite-only
	if (channel->isFull()) {
		client->addReply(server_->getHostname(), "471", ERR_CHANNELISFULL(chanName));
	} else if (channel->checkIfBanned(client->getNickname())) {
		client->addReply(server_->getHostname(), "474", ERR_BANNEDFROMCHAN(chanName));
	} else if (channel->getKey() != key) {
		client->addReply(server_->getHostname(), "475", ERR_BADCHANNELKEY(chanName, key));
	} else {
		client->joinChannel(channel);
		std::string &topic = channel->getTopic();
		if (topic == "")
			client->addReply(RPL_NOTOPIC(chanName));
		else
			client->addReply(RPL_TOPIC(chanName, topic));
	}
}
