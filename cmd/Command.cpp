#include "Command.hpp"

Command::Command() : authRequired_(true) {}
Command::Command(bool auth, Server *server) : authRequired_(auth), server_(server) {}
//Command::Command(bool auth, Server *server, std::string serv_host) : authRequired_(auth), server_(server),
//																		serv_hostname(serv_host) {}
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
