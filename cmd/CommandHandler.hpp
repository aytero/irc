#ifndef IRC_COMMANDHANDLER_HPP
# define IRC_COMMANDHANDLER_HPP

class CommandHandler;

# include <string>
# include <vector>
# include <map>
# include <sstream>
# include <iostream>

# include "Command.hpp"
//# include "../core/Client.hpp"

//class Server;

class CommandHandler {
	std::map<std::string, Command*> commands;
	std::vector<std::string> parse(std::string &message);
//	Server *server_;
    std::string serv_host;

public:
	CommandHandler();
	CommandHandler(Server *server);
	~CommandHandler();
	void handle(Client *client, std::string &message);

};


#endif //IRC_COMMANDHANDLER_HPP
