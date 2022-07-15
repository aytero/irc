#ifndef IRC_COMMANDS_HPP
# define IRC_COMMANDS_HPP

# include "Command.hpp"
# include "replies.hpp"

//class Command;

class JoinCommand : public Command {
public:
	JoinCommand(bool auth);
//	JoinCommand(const JoinCommand &ref);
	~JoinCommand();
//	JoinCommand &operator=(const JoinCommand &ref);

	void execute(Server *server, Client *client, std::vector<std::string> args);
};

class PingCommand : public Command {
public:
	PingCommand(bool auth);
//	PingCommand(const PingCommand &ref);
	~PingCommand();
//	PingCommand &operator=(const PingCommand &ref);

	void execute(Server *server, Client *client, std::vector<std::string> args);
};

class NamesCommand : public Command {
public:
	void execute(Server *server, Client *client, std::vector<std::string> args);
};

class PartCommand : public Command {
public:
	void execute(Server *server, Client *client, std::vector<std::string> args);
};

class ModeCommand : public Command {
public:
	void execute(Server *server, Client *client, std::vector<std::string> args);
};
#endif //IRC_COMMANDS_HPP
