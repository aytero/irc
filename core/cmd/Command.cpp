#include "Command.hpp"

Command::Command(bool auth) : authRequired_(auth) {}
Command::Command(const Command &ref) {}
Command &Command::operator=(const Command &ref) {}
Command::~Command() {}

bool Command::authRequired() const {
	return authRequired_;
}
