#ifndef IRC_CHANNEL_HPP
# define IRC_CHANNEL_HPP

# include <string>
# define SERVER_MAX_CHANNELS 50
# define USER_MAX_CHANNELS 10
# define MAX_USERS

class Channel {
	std::string name;
	std::string key;
public:
	Channel();
	~Channel();

	std::string getName();
	bool isFull();
	bool checkIfBanned(std::string nick);
	std::string getKey();
};


#endif //IRC_CHANNEL_HPP
