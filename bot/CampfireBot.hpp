#ifndef CAMPFIREBOT_HPP
# define CAMPFIREBOT_HPP

# include <string>

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include <cstdlib>
# include <ctime>

# include <vector>

# include "../logger/Logger.hpp"

// todo: rename, PING-PONG, cmds, join channel

# define MAX_TRIES 5

enum BotState {
	RUN,
	GAME,
};

class CampfireBot {
	int sock;
	bool stop;
	BotState state_;

	std::string channel_;


	int tries_;
	int num_;
	bool finishGame;
	void initGuessNumber();
	void checkGuess(int guess);

	void handle(std::string message);
	void sendReply(std::string mes);

	void gameCmd();
	void finishCmd();

public:
	CampfireBot();
	CampfireBot(std::string port, std::string pass);
	~CampfireBot() {}

	void run();

};


#endif //CAMPFIREBOT_HPP
