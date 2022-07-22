#include "Command.hpp"

#define KICK_DEFAULT "No reason specified"

//ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//ERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL

KickCommand::KickCommand(bool auth, Server *server) : Command(auth, server) {}

void KickCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.size() < 2) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("KICK")));
		return;
	}

	std::string &chan_name = args[0];
	std::string &target_name = args[1];

	Channel *channel = client->getChannel(chan_name);
	if (!channel) {
		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(chan_name));
		return;
	}
	if (!channel->isOp(client)) {
		client->setReply(server_->getHostname(), ERR_CHANOORIVSNEEDED(chan_name));
		return;
	}

	Client *target = server_->getClient(target_name);
	if (!target) {
		client->setReply(server_->getHostname(), ERR_NOSUCHNICK(target_name));
		return;
	}
	if (!target->getChannel(chan_name)) {
		client->setReply(server_->getHostname(), ERR_USERNOTINCHANNEL(target_name, chan_name));
		return;

	}

	std::string reason = "";
	if (atgs.size() >= 3) {
		for (int i = 2; i < args.size(); ++i) {
			reason.append(args[i] + " "); // SP after the last word -.-
		}
	} else
		reason = KICK_DEFAULT;

	channel->kick(client, target, reason);
}
