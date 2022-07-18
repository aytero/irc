#include "Command.hpp"

QuitCommand::QuitCommand(bool auth, Server *server) : Command(auth, server) {}

void QuitCommand::execute(Client *client, std::vector <std::string> args) {
	std::string reason = "Disconnected."; // default reason
	if (!args.empty())
		reason = args.[0];
	client->addReply(RPL_QUIT(reason));
	//disconnect from the serv?
}