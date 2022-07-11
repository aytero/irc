//
// Created by Asiet Dzharimova on 11.07.2022.
//

#include "commands.hpp"

// JOIN
JoinCommand::JoinCommand() {}
JoinCommand::JoinCommand(const JoinCommand &ref) {}
JoinCommand::~JoinCommand() {}
JoinCommand &JoinCommand::operator=(const JoinCommand &ref) {}

void JoinCommand::execute(Client *client, std::vector<std::string> args) {}

// PING
PingCommand::PingCommand() {}
PingCommand::PingCommand(const PingCommand &ref) {}
PingCommand::~PingCommand() {}
PingCommand &PingCommand::operator=(const PingCommand &ref) {}

void PingCommand::execute(Client *client, std::vector<std::string> args) {
	client.reply("PING");
}
