#ifndef IRC_COMMANDHANDLER_HPP
# define IRC_COMMANDHANDLER_HPP

# include "../Client.hpp"

class CommandHandler {

public:
	void handle(Client *client, std::string &message);
};


#endif //IRC_COMMANDHANDLER_HPP
