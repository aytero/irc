#ifndef IRC_COMMAND_HPP
# define IRC_COMMAND_HPP

# include <iostream>
# include <string>
# include <vector>
# include "../core/Client.hpp"

class Command {
protected:
	bool authRequired_;
//	Server *server;

public:
	Command();
	Command(bool auth);
	Command(const Command &ref);
	Command &operator=(const Command &ref);
	virtual ~Command();

	virtual void execute(Client *client, std::vector<std::string> args) = 0;
	bool authRequired() const;
};


#endif //IRC_COMMAND_HPP
