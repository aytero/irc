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

# include "Message.hpp"
# include "../logger/Logger.hpp"

//#define RED 	"\033[1;31m"
#define GREEN   "\033[0;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define PURPLE  "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET_B   "\033[0;0m"
#define BOLD    "\033[;1m"
#define REVERSE "\033[;7m"
# define ORG "\e[38;5;173m"
# define B_ORG "\e[38;5;166m"
//# define WHT "\e[0;37m"
//# define GREY "\e[38;5;241m"
//# define RESET "\033[0m\n"

// todo: colors

# define MAX_TRIES 7

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

	void helpCmd();
	void gameCmd();
	void finishCmd();

public:
	CampfireBot();
	CampfireBot(std::string port, std::string pass);
	~CampfireBot() {}

	void run();

};


#endif //CAMPFIREBOT_HPP
