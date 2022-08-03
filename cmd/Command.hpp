#ifndef IRC_COMMAND_HPP
# define IRC_COMMAND_HPP

class Command;

# include <iostream>
# include <string>
# include <vector>
# include "replies.hpp"
# include "../core/Utils.hpp"
//# include "../logger/Logger.hpp"

# include "../core/Server.hpp"
//class Server;


// todo: Mode Invite
//  done: Kick Notice Topic Oper Kill List Ping Pong
// Oper -> Squit && Connect are used for server-to-server communication

class Command {
protected:
	bool authRequired_;
	Server *server_;
//	std::string serv_hostname;

public:
	Command();
	Command(bool auth, Server *server);
//	Command(bool auth, Server *server, std::string serv_host);
	Command(const Command &ref);
	Command &operator=(const Command &ref);
	virtual ~Command();

	virtual void execute(Client *client, std::vector<std::string> args) = 0;
	bool authRequired() const;
//	friend std::string &getHostname()
};


/*
* * * 		Connection registration		* * *
*/
class PassCmd : public Command {
//	friend void Server::checkConnectionRegistration(Client *client);
public:
	PassCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class NickCmd : public Command {
	bool validate(std::string nickname);
public:
	NickCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class UserCmd : public Command {
public:
	UserCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class QuitCmd : public Command {
public:
	QuitCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};


/*
* * * 		Miscellaneous messages		* * *
*/
class ModeCmd : public Command {
	void userMode(Client *client, std::vector <std::string> args);
	void chanMode(Client *client, std::vector <std::string> args);

public:
	ModeCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class KillCmd : public Command {
public:
	KillCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class PingCmd : public Command {
public:
	PingCmd(bool auth, Server *server);
	~PingCmd();

	void execute(Client *client, std::vector<std::string> args);
};

class PongCmd : public Command {
public:
	PongCmd(bool auth, Server *server);
	~PongCmd();

	void execute(Client *client, std::vector<std::string> args);
};

class MotdCmd : public Command {
	std::string getMOTD();
public:
	MotdCmd(bool auth, Server *server);
	~MotdCmd();

	void execute(Client *client, std::vector<std::string> args);
};

//class NamesCmd : public Command {
//public:
//	NamesCmd(bool auth, Server *server);
//	void execute(Client *client, std::vector<std::string> args);
//};


/*
* * * 		Channel operations		* * *
*/
class JoinCmd : public Command {
public:
	JoinCmd(bool auth, Server *server);
	//	JoinCmd(const JoinCmd &ref);
	~JoinCmd();
	//	JoinCmd &operator=(const JoinCmd &ref);

	void execute(Client *client, std::vector<std::string> args);
};

class PartCmd : public Command {
public:
	PartCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class OperCmd : public Command {
public:
	OperCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class KickCmd : public Command {
public:
	KickCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class TopicCmd : public Command {
public:
	TopicCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class ListCmd : public Command {
public:
	ListCmd(bool auth, Server *server);
	~ListCmd();
	void execute(Client *client, std::vector<std::string> args);
};


/*
* * * 		Sending messages		* * *
*/
class PrivMsgCmd : public Command {
public:
	PrivMsgCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class NoticeCmd : public Command {
public:
	NoticeCmd(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};


#endif //IRC_COMMAND_HPP
