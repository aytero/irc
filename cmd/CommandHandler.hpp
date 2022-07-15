#ifndef IRC_COMMANDHANDLER_HPP
# define IRC_COMMANDHANDLER_HPP

# include <string>
# include <vector>
# include <map>
# include <sstream>
# include <iostream>
# include "Command.hpp"
# include "commands.hpp"
# include "../core/Client.hpp"

class Command;

class CommandHandler {
	std::map<std::string, Command*> commands;
	std::vector<std::string> parse(std::string &message);

public:
	CommandHandler();
	~CommandHandler();
	void handle(Server *server, Client *client, std::string &message);
};


#endif //IRC_COMMANDHANDLER_HPP
