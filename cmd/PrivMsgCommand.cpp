#include "Command.hpp"

PrivMsgCommand::PrivMsgCommand(bool auth, Server *server) : Command(auth, server) {}

void PrivMsgCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.size() < 2) {
		client->addReply(ERR_NEEDMOREPARAMS(std::string("PrivMsg")));
		return;
	}
	std::string targetName = args[0];
	std::string message;
	for (int i = 1; i < args.size(); ++i)
		message.append(args[i] + " ");
	if (message.empty()) {
		client->addReply(server_->getHostname(), "412", ERR_NOTEXTTOSEND());
		return;
	}
//	if (targetName[0] == '#') {
//		Channel *channel = server_->getChannel(targetName.substr(1));
//		if (!channel) {
//			client->addReply(server_->getHostname(), "403", ERR_NOSUCHCHANNEL(targetName));
//			return;
//		}
//		// if user not in chan
//		if (channel->getModeN() && channel->findUser(client->getNickname()) == 0) {
//			client->addReply(server_->getHostname(), "404", ERR_CANNOTSENDTOCHAN(targetName));
//			return;
//		}
//		channel->broadcast(RPL_PRIVMSG(client->getPrefix(), message), client);
//		return;
//	}
	Client *target = server_->getClient(targetName);
	if (!target) {
		client->addReply(server_->getHostname(), "401", ERR_NOSUCHNICK(targetName));
		return;
	}
	target->addReply(RPL_PRIVMSG(client->getPrefix(), message));
	// target add write event
	server_->addEvent(WRITE_EVENT, target->getFd());
}