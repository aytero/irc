#include "Command.hpp"
#include "../core/Channel.hpp"
//#include "../core/Server.hpp"

JoinCmd::JoinCmd(bool auth, Server *server) : Command(auth, server) {}
//JoinCmd::JoinCmd(const JoinCmd &ref) {}
JoinCmd::~JoinCmd() {}
//JoinCmd &JoinCmd::operator=(const JoinCmd &ref) {}

void JoinCmd::execute(Client *client, std::vector<std::string> args) {
	if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("JOIN")));
		return;
	}
	if (args[0] == "0") {
		std::map<std::string,Channel*> chans = client->getAllChannels();
		// PART in a loop
		std::map<std::string,Channel*>::iterator it = chans.begin();
		for (; it != chans.end(); ++it) { 
			const std::string &chanName = it->first;
			//client->addReply(RPL_PART(client->getPrefix(), it->first, std::string("just left")));
			it->second->broadcast(RPL_PART(client->getPrefix(), chanName, std::string("just left")));
//			client->leaveChannel(it->second);
//			logger::info(SSTR("Channel: " << chanName << " Users: " << it->second->getUserNum()));
//			if (it->second->getUserNum() == 0)
				//it->second->del()
//				server_->deleteChannel(chanName);
		}
		client->leaveAllChannels();
		server_->broadcastEvent(client);
//		if no users delete chan?
		return;
	}
	std::string chanName = args[0];
	std::string key = "";
	if (args.size() > 1)
		key = args[1];
	if (server_->getChannelNum() > SERVER_MAX_CHANNELS || client->getChannelNum() > USER_MAX_CHANNELS) {
//		client->addReply(server_->getHostname(), ERR_TOOMANYCHANNELS(chanName));
		client->addReply(server_->getHostname(), "405", ERR_TOOMANYCHANNELS(chanName));
		return;
	}

	Channel *channel = server_->getChannel(chanName);
//	logger::debug(SSTR(chanName << " channel name (JOIN)"));
	if (chanName.size() < 2 || (chanName[0] != '#' && chanName[0] != '&')) {
		client->addReply(server_->getHostname(), ERR_NOSUCHCHANNEL(client->getNickname(), chanName));
		return;
	}
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
		if (client->joinChannel(channel)) {
			logger::info(SSTR("Channel: " << chanName << " Users: " << channel->getUserNum()));
			channel->broadcast(RPL_JOIN(client->getPrefix(), chanName));
			// also brodcast names list
			server_->broadcastEvent();
			std::string &topic = channel->getTopic();
			if (topic == "")
				client->addReply(server_->getHostname(), RPL_NOTOPIC(client->getNickname(), chanName));
			else
				client->addReply(server_->getHostname(), RPL_TOPIC(client->getNickname(), chanName, topic));
		}
	}
}
