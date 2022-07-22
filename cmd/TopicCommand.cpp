#include "Command.hpp"

//ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//RPL_NOTOPIC                     RPL_TOPIC
//ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

TopicCommand::TopicCommand(bool auth, Server *server) : Command(auth, server) {}

void TopicCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.empty()) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("TOPIC")));
		return;
	}

	std::string &name = args[0];
	Channel *channel = client->getChannel(name);
	if (!channel) {
		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(name));
		return;
	}

	std::string &topic = channel->getTopic();
	if (args.size() == 1) {
		if (!topic || topic == "")
			client->addReply(RPL_NOTOPIC(name))
		else
			client->addReply(RPL_TOPIC(name, topic))
	} else {
		/// check mode
		if (!channel->isOp(client)) {
			client->setReply(server_->getHostname(), ERR_CHANOORIVSNEEDED(name));
			return;
		}
		topic.clear();
		for (int i = 1; i < args.size(); ++i) {
			topic.append(args[i] + " "); // SP after the last word -.-
		}
		channel->setTopic(topic);
		channel->broadcast(":" + client->getPrefix() + " TOPIC " + RPL_TOPIC(name, topic), client);
	}

}
