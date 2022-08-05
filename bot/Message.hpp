#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <cstring>
# include <queue>
# include <vector>

# include "../logger/Logger.hpp"

class Message {
	std::string prefix;
	std::string serv_command;
	std::string chan;
	std::vector<std::string> args;

	Message();
	Message(const Message &ref);
	Message &operator=(const Message &ref);

	void parse(std::string &s);
public:
	Message(std::string &s); // const
//	Message(const char *s);
	~Message();

	std::string &getPrefix();
	std::string &getServCommand();
	std::string &getChannel();
	std::vector<std::string> &getArgs(); // & const () const
};


#endif //MESSAGE_HPP
