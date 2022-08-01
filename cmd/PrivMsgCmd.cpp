#include "Command.hpp"

PrivMsgCmd::PrivMsgCmd(bool auth, Server *server) : Command(auth, server) {}

void PrivMsgCmd::execute(Client *client, std::vector <std::string> args) {
	if (args.size() == 0) {
		client->addReply(server_->getHostname(), ERR_NORECIPIENT(client->getNickname(), std::string("PRIVMSG")));
		return;
	}
//	if (message.empty()) {
	if (args.size() == 1) {
		client->addReply(server_->getHostname(), ERR_NOTEXTTOSEND(client->getNickname()));
		return;
	}
	std::string targetName = args[0];
//	args.erase(args.begin());
	std::string message = utils::vect_to_string(args, 1);

	logger::debug(SSTR("PRIVMSG target: " << targetName));

	if (targetName[0] == '#' || targetName[0] == '&') {
		logger::debug("broadcast");
		Channel *channel = server_->getChannel(targetName);
		if (!channel) {
			client->addReply(server_->getHostname(), ERR_NOSUCHCHANNEL(client->getNickname(), targetName));
			logger::debug(ERR_NOSUCHCHANNEL(client->getNickname(), targetName));
			return;
		}
		// if user not in chan
		if (!channel->outsideMessageAllowed() && channel->findUser(client->getNickname()) == 0) {
			client->addReply(server_->getHostname(), ERR_CANNOTSENDTOCHAN(targetName));
			logger::debug(ERR_CANNOTSENDTOCHAN(targetName));
			return;
		}
		channel->broadcast(RPL_PRIVMSG(client->getPrefix(), targetName, message));
//		channel->broadcast(RPL_PRIVMSG(client->getPrefix(), targetName, message), client);
		server_->broadcastEvent(channel->getUsers());
		return;
	}
	Client *target = server_->getClient(targetName);
	if (!target) {
		client->addReply(server_->getHostname(), ERR_NOSUCHNICK(targetName));
		logger::debug(ERR_NOSUCHNICK(targetName));
		return;
	}
//	target->addReply(":" + client->getPrefix() + " PRIVMSG " + targetName + " :" + message);
	target->addReply(RPL_PRIVMSG(client->getPrefix(), targetName, message));
	server_->addEvent(WRITE_EVENT, target->getFd());
}
