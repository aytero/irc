#include "Command.hpp"

//ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//RPL_NOTOPIC                     RPL_TOPIC
//ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

TopicCmd::TopicCmd(bool auth, Server *server) : Command(auth, server) {}

void TopicCmd::execute(Client *client, std::vector <std::string> args) {
	if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("TOPIC")));
		return;
	}

	std::string &name = args[0];
	Channel *channel = client->getChannel(name);
	if (!channel) {
		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(client->getNickname(), name));
		return;
	}

	std::string &topic = channel->getTopic();
	if (args.size() == 1) {
		if (topic.empty() || topic == "")
			client->addReply(server_->getHostname(), RPL_NOTOPIC(client->getNickname(), name));
		else
			client->addReply(server_->getHostname(), RPL_TOPIC(client->getNickname(), name, topic));
	} else {
		/// check mode
		if (!channel->isOp(client)) {
			client->addReply(server_->getHostname(), ERR_CHANOPRIVSNEEDED(name));
			return;
		}
		topic.clear();
		topic = utils::vect_to_string(args, 1);
		channel->setTopic(topic);
		channel->broadcast(":" + client->getPrefix() + " TOPIC " + RPL_TOPIC(client->getNickname(), name, topic));
		server_->broadcastEvent(channel->getUsers());
	}

}
