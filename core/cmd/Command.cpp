#include "Command.hpp"

Command::Command(bool auth = true, Server *serv) : authRequired_(auth), server(serv) {}
Command::Command(const Command &ref) {}
Command &Command::operator=(const Command &ref) {}
Command::~Command() {}

bool Command::authRequired() const {
	return authRequired_;
}
