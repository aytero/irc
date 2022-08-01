#include "Command.hpp"

NamesCmd::NamesCmd(bool auth, Server *server) : Command(auth, server) {}

void NamesCmd::execute(Client *client, std::vector<std::string> args) {
	std::vector<Channel*>::iterator it = client->getChannels().begin();
	std::vector<Channel*>::iterator ite = client->getChannels().end();
	std::string repl;
	for (; it != ite; ++it) {
		repl.append("dd");
	}
	client->addReply(repl);
}
