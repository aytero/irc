#include "commands.hpp"

// JOIN
JoinCommand::JoinCommand(bool auth) : Command(auth) {}
//JoinCommand::JoinCommand(const JoinCommand &ref) {}
JoinCommand::~JoinCommand() {}
//JoinCommand &JoinCommand::operator=(const JoinCommand &ref) {}

void JoinCommand::execute(Client *client, std::vector<std::string> args) {}

// PING
PingCommand::PingCommand(bool auth) : Command(auth) {}
//PingCommand::PingCommand(const PingCommand &ref) {}
PingCommand::~PingCommand() {}
//PingCommand &PingCommand::operator=(const PingCommand &ref) {}

void PingCommand::execute(Client *client, std::vector<std::string> args) {
	client->setReply("PING");
}
