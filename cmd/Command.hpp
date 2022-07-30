#ifndef IRC_COMMAND_HPP
# define IRC_COMMAND_HPP

class Command;

# include <iostream>
# include <string>
# include <vector>
# include "replies.hpp"
//# include "../logger/Logger.hpp"

# include "../core/Server.hpp"


// todo: Ping Pong Mode Invite
//  done: Kick Notice Topic Oper Kill List
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

class JoinCommand : public Command {
public:
	JoinCommand(bool auth, Server *server);
	//	JoinCommand(const JoinCommand &ref);
	~JoinCommand();
	//	JoinCommand &operator=(const JoinCommand &ref);

	void execute(Client *client, std::vector<std::string> args);
};

class List : public Command {
public:
	List(bool auth, Server *server);
	~List();

	void execute(Client *client, std::vector<std::string> args);
};

class PingCommand : public Command {
public:
	PingCommand(bool auth, Server *server);
	~PingCommand();

	void execute(Client *client, std::vector<std::string> args);
};

//class PongCommand : public Command {
//public:
//	PongCommand(bool auth, Server *server);
//	~PongCommand();
//
//	void execute(Client *client, std::vector<std::string> args);
//};

//class NamesCommand : public Command {
//public:
//	NamesCommand(bool auth, Server *server);
//	void execute(Client *client, std::vector<std::string> args);
//};

class PartCommand : public Command {
public:
	PartCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class ModeCommand : public Command {
	void userMode(Client *client, std::vector <std::string> args);
	void chanMode(Client *client, std::vector <std::string> args);

public:
	ModeCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class OperCommand : public Command {
public:
	OperCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class KickCommand : public Command {
public:
	KickCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class TopicCommand : public Command {
public:
	TopicCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class PrivMsgCommand : public Command {
public:
	PrivMsgCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class NoticeCommand : public Command {
public:
	NoticeCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class PasswordCommand : public Command {
public:
	PasswordCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class NickCommand : public Command {
    void welcome(Client *client);
	bool validate(std::string nickname);
    std::string getMOTD();
public:
	NickCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class UserCommand : public Command {
public:
	UserCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class QuitCommand : public Command {
public:
	QuitCommand(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

class Kill : public Command {
public:
	Kill(bool auth, Server *server);
	void execute(Client *client, std::vector<std::string> args);
};

#endif //IRC_COMMAND_HPP
