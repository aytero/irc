#ifndef IRC_COMMANDHANDLER_HPP
# define IRC_COMMANDHANDLER_HPP

# include <string>
# include <vector>
# include <map>
# include <sstream>
# include <iostream>

# include "Command.hpp"
# include "../core/Server.hpp"

class Command;
class Server;

class CommandHandler {
	std::map<std::string, Command*> commands;
	std::vector<std::string> parse(std::string &message);
	Server *server_;

public:
	CommandHandler();
	CommandHandler(Server *server);
	~CommandHandler();
	void handle(Client *client, std::string &message);

};


#endif //IRC_COMMANDHANDLER_HPP
