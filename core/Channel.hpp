#ifndef IRC_CHANNEL_HPP
# define IRC_CHANNEL_HPP

# include <string>
# include <vector>
# include "Server.hpp"

# define SERVER_MAX_CHANNELS 50
# define USER_MAX_CHANNELS 10
# define MAX_USERS

class Client;

class Channel {
	std::string name;
	std::string key;
	int userNum;
	int maxUserNum;
	std::vector<Client*> users;
	std::vector<std::string> banlist;

public:
	Channel(std::string name, std::string key);
	Channel();
	Channel(const Channel &ref);
	~Channel();
	Channel &operator=(const Channel &ref);

	std::string getName();
	bool isFull();
	bool checkIfBanned(std::string nick);
	std::string getKey();
	void setOp(Client *client) {}
};


#endif //IRC_CHANNEL_HPP
