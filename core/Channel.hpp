#ifndef IRC_CHANNEL_HPP
# define IRC_CHANNEL_HPP

class Channel;

# include <string>
# include <vector>
# include <algorithm>

# define SERVER_MAX_CHANNELS 50
# define USER_MAX_CHANNELS 10
# define MAX_USERS

# include "Client.hpp"

class Channel {
	std::string name;
	std::string key;
	int userNum;
	int maxUserNum;
	std::vector<Client*> users;
	std::vector<std::string> banlist;

	Client *creator;
	std::vector<Client*> operators;
	std::string topic;

	bool modeN;

public:
	Channel(std::string name, std::string key);
	Channel();
	Channel(const Channel &ref);
	~Channel();
	Channel &operator=(const Channel &ref);

	void broadcast(std::string mes, Client *exclude = 0);
	void kick(Client *op, Client *target, const std::string &reason);
	std::string &getName();
	bool isFull();
	bool checkIfBanned(std::string nick);
	std::string &getKey();
	void setTopic(std::string &topic);
	bool isOp(const Client *client);
	void setOp(Client *client);
	void addUser(Client *client);
	std::vector<Client*> getUsers();
	std::string &getTopic();
	bool outsideMessageAllowed();
	Client *findUser(std::string &nick);
	void removeUser(std::string &nick);
};


#endif //IRC_CHANNEL_HPP
