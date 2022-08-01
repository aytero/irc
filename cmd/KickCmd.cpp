#include "Command.hpp"

#define KICK_DEFAULT "No reason specified"

//ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//ERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL

KickCmd::KickCmd(bool auth, Server *server) : Command(auth, server) {}

void KickCmd::execute(Client *client, std::vector <std::string> args) {
	if (args.size() < 2) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("KICK")));
		return;
	}

	std::string &chan_name = args[0];
	std::string &target_name = args[1];

	Channel *channel = client->getChannel(chan_name);
	if (!channel) {
		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(client->getNickname(), chan_name));
		return;
	}
	if (!channel->isOp(client)) {
		client->addReply(server_->getHostname(), ERR_CHANOPRIVSNEEDED(chan_name));
		return;
	}

	Client *target = server_->getClient(target_name);
	if (!target) {
		client->addReply(server_->getHostname(), ERR_NOSUCHNICK(target_name));
		return;
	}
	if (!target->getChannel(chan_name)) {
		client->addReply(server_->getHostname(), ERR_USERNOTINCHANNEL(target_name, chan_name));
		return;

	}

	std::string reason = KICK_DEFAULT;
	if (args.size() >= 3) {
		reason = utils::vect_to_string(args, 2);
	}

	server_->broadcastEvent(channel->getUsers());
	channel->kick(client, target, reason);
}
