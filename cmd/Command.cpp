#include "Command.hpp"

Command::Command() : authRequired_(true) {}
Command::Command(bool auth) : authRequired_(auth) {}
Command::Command(const Command &ref) : authRequired_(ref.authRequired_) {}
Command &Command::operator=(const Command &ref) {
	if (this != &ref)
		authRequired_ = ref.authRequired_;
	return *this;
}
Command::~Command() {}

bool Command::authRequired() const {
	return authRequired_;
}
