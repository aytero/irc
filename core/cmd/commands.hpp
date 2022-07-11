#ifndef IRC_COMMANDS_HPP
# define IRC_COMMANDS_HPP

# include "Command.hpp"

class JoinCommand : public Command {
public:
	JoinCommand();
	JoinCommand(const JoinCommand &ref);
	~JoinCommand();
	JoinCommand &operator=(const JoinCommand &ref);

	void execute(Client *client, std::vector<std::string> args);
};

class PingCommand : public Command {
public:
	PingCommand();
	PingCommand(const PingCommand &ref);
	~PingCommand();
	PingCommand &operator=(const PingCommand &ref);

	void execute(Client *client, std::vector<std::string> args);
};

#endif //IRC_COMMANDS_HPP
