#include "Command.hpp"

NoticeCmd::NoticeCmd(bool auth, Server *server) : Command(auth, server) {}

void NoticeCmd::execute(Client *client, std::vector <std::string> args) {
	if (args.size() < 2) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("NOTICE")));
		return;
	}
	std::string targetName = args[0];
	std::string message = utils::vect_to_string(args, 1);

	if (message.empty()) {
		client->addReply(server_->getHostname(), ERR_NOTEXTTOSEND(client->getNickname()));
		return;
	}

	logger::debug(SSTR("NOTICE target: " << targetName));
	if (targetName[0] == '#' || targetName[0] == '&') {
		logger::debug("broadcast");
		Channel *channel = server_->getChannel(targetName);
		if (!channel) {
			client->addReply(server_->getHostname(), ERR_NOSUCHCHANNEL(client->getNickname(), targetName));
			return;
		}
		// if user not in chan
		if (!channel->outsideMessageAllowed() && channel->findUser(client->getNickname()) == 0) {
			client->addReply(server_->getHostname(), ERR_CANNOTSENDTOCHAN(targetName));
			logger::debug(ERR_CANNOTSENDTOCHAN(targetName));
			return;
		}
		channel->broadcast(RPL_PRIVMSG(client->getPrefix(), targetName, message), client);
		server_->broadcastEvent(channel->getUsers());
		return;
	}
	Client *target = server_->getClient(targetName);
	if (!target) {
		client->addReply(server_->getHostname(), ERR_NOSUCHNICK(targetName));
		return;
	}
	target->addReply(RPL_NOTICE(client->getPrefix(), targetName, message));
	// target add write event
	server_->addEvent(WRITE_EVENT, target->getFd());
}
