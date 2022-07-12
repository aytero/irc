#ifndef IRC_COMMANDHANDLER_HPP
# define IRC_COMMANDHANDLER_HPP

# include <string>
# include <vector>
# include <map>
# include <sstream>
# include "Command.hpp"
# include "commands.hpp"
# include "../Client.hpp"

class Command;

class CommandHandler {
	std::map<std::string, Command*> commands;

	std::vector<std::string> parse(std::string &message);
public:
	CommandHandler();
	~CommandHandler();
	void handle(Client *client, std::string &message);
};


#endif //IRC_COMMANDHANDLER_HPP
