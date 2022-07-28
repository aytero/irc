#include "Command.hpp"

//   Numeric Replies:
//
//   ERR_TOOMANYMATCHES              ERR_NOSUCHSERVER
//   RPL_LIST                        RPL_LISTEND
//
//   Examples:
//
//   LIST                            ; Command to list all channels.
//
//   LIST #twilight_zone,#42         ; Command to list channels
//                                     #twilight_zone and #42


List::List(bool auth, Server *server) : Command(auth, server) {}
List::~List() {}

void List::execute(Client *client, std::vector <std::string> args) {
	std::vector<Channel*> chans = server_->getChannels();
	std::vector<Channel*>::iterator it = chans.begin();
	std::string &from = server_->getHostname();
	client->addReply(from, RPL_LIST_HINT());
	for (; it != chans.end(); ++it) {
		client->addReply(from, RPL_LIST((*it)->getName(), std::to_string((*it)->getUsers().size()), (*it)->getTopic()));
	}
//	for (int i = 0; i < server_->getChannelNum(); ++i) {
//		Channel *chan = server_->getChannel(i);
//		client->addReply(server_->getHostname(), RPL_LIST(chan->getName(), "", chan->getTopic()))
//	}
	client->addReply(from, RPL_LISTEND());
}
