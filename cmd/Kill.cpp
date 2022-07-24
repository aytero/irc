#include "Command.hpp"

//ERR_NOPRIVILEGES              ERR_NEEDMOREPARAMS
//ERR_NOSUCHNICK                ERR_CANTKILLSERVER

Kill::Kill(bool auth, Server *server) : Command(auth, server) {}

void Kill::execute(Client *client, std::vector <std::string> args) {
	if (args.size() < 1) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("KILL")));
		return;
	}

	if (!server_->isOp(client->getOpername())) {
		client->addReply(server_->getHostname(), ERR_NOPRIVILEGES());
		return;
	}

	std::string &target_name = args[0];
	Client *target = server_->getClient(target_name);
	if (!target) {
		client->addReply(server_->getHostname(), ERR_NOSUCHNICK(target_name));
		return;
	}

	std::string reason;
	if (args.size() > 1) {
		for (int i = 1; i < args.size(); ++i)
			reason.append(args[i] + " "); // SP after last word _._
	} else {
		reason = "No reason specified"; // default reason
	}
	// oper prefix
	target->addReply(client->getOpername(), RPL_KILL(target_name, reason));
	server_->ban(target_name);
	target->quit();
	server_->addEvent(WRITE_EVENT, target->getFd());




//	std::string &chan_name = args[0];
//	std::string &target_name = args[1];
//
//	Channel *channel = client->getChannel(chan_name);
//	if (!channel) {
//		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(chan_name));
//		return;
//	}
//	if (!channel->isOp(client)) {
//		client->addReply(server_->getHostname(), ERR_CHANOPRIVSNEEDED(chan_name));
//		return;
//	}
//
//	Client *target = server_->getClient(target_name);
//	if (!target) {
//		client->addReply(server_->getHostname(), ERR_NOSUCHNICK(target_name));
//		return;
//	}
//	if (!target->getChannel(chan_name)) {
//		client->addReply(server_->getHostname(), ERR_USERNOTINCHANNEL(target_name, chan_name));
//		return;
//
//	}
//	std::string reason = "";
//	if (args.size() >= 3) {
//		for (int i = 2; i < args.size(); ++i) {
//			reason.append(args[i] + " "); // SP after the last word -.-
//		}
//	} else
//		reason = KICK_DEFAULT;
//
//	server_->broadcastEvent(channel->getUsers());
//	channel->kick(client, target, reason);
}
