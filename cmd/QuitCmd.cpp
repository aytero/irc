#include "Command.hpp"

# define QUIT_DEFAULT "Disconnected."

QuitCmd::QuitCmd(bool auth, Server *server) : Command(auth, server) {}

void QuitCmd::execute(Client *client, std::vector <std::string> args) {
	std::string reason = QUIT_DEFAULT;
	if (!args.empty())
		reason = utils::vect_to_string(args);

//	client->addReply(server_->getHostname(), RPL_QUIT(client->getPrefix(), reason));
	server_->broadcastMessage(RPL_QUIT(client->getPrefix(), reason));
	server_->broadcastEvent(client);
//	client->addReply(server_->getHostname(), RPL_QUIT(client->getPrefix(), reason));
	client->quit();
}