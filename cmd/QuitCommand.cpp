#include "Command.hpp"

QuitCommand::QuitCommand(bool auth, Server *server) : Command(auth, server) {}

void QuitCommand::execute(Client *client, std::vector <std::string> args) {
	std::string reason;
	if (!args.empty()) {
		for (int i = 0; i < args.size(); ++i)
			reason.append(args[i] + " "); // SP after last word _._
	} else
		reason = "Disconnected."; // default reason
	client->addReply(server_->getHostname(), RPL_QUIT(client->getPrefix(), reason));
	client->quit();
}