#include "Command.hpp"

PrivMsgCommand::PrivMsgCommand(bool auth, Server *server) : Command(auth, server) {}

void PrivMsgCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.size() < 2) {
		client->addReply(ERR_NEEDMOREPARAMS("PrivMsg"));
		return;
	}
	std::string targetName = args[0];
	std::string message;
	for (int i = 1; i < args.size(); ++i)
		message.append(args[i]);
	if (message.empty()) {
		client->addReply(ERR_NOTEXTTOSEND());
		return;
	}
	if (targetName[0] == '#') {
		Channel *channel = server_->getChannel(targetName.substr(1));
		if (!channel) {
			client->addReply(ERR_NOSUCHCHANNEL());
			return;
		}
		// if user not in chan
		if (channel->getModeN() && channel->findUser(client->getNickname()) == 0) {
			client->addReply(ERR_CANNOTSENDTOCHAN());
			return;
		}
		channel->broadcast(RPL_PRIVMSG(client->getPrefix(), targetName, message), client);
		return;
	}
	Client *target = server->getClient(targetName);
	if (!target) {
		client->addReply(ERR_NOSUCHNICK(target));
		return;
	}
	target->addReply(RPL_PRIVMSG(client->getPrefix(), targetName, message));
	// target add write event
}