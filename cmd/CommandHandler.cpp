#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {}

CommandHandler::CommandHandler(Server *server) : serv_host(server->getHostname()) {
	commands["PASS"] = new PassCmd(false, server);
	commands["NICK"] = new NickCmd(false, server);
	commands["USER"] = new UserCmd(false, server);

	commands["JOIN"] = new JoinCmd(true, server);
	commands["PART"] = new PartCmd(true, server);
	commands["PRIVMSG"] = new PrivMsgCmd(true, server);
	commands["NOTICE"] = new NoticeCmd(true, server);
	commands["QUIT"] = new QuitCmd(true, server);
	commands["PING"] = new PingCmd(true, server);
//	commands["PONG"] = new PongCmd(true, server);
	commands["TOPIC"] = new TopicCmd(true, server);
	commands["MODE"] = new ModeCmd(true, server);
	commands["OPER"] = new OperCmd(true, server);
	commands["KICK"] = new KickCmd(true, server);
	commands["KILL"] = new KillCmd(true, server);
	commands["LIST"] = new ListCmd(true, server);
}
//CommandHandler::CommandHandler() {}
CommandHandler::~CommandHandler() {
	std::map<std::string, Command*>::iterator it = commands.begin();
	std::map<std::string, Command*>::iterator ite = commands.end();
	for (; it != ite; ++it) {
		delete it->second;
	}
	commands.clear();
}
//CommandHandler &CommandHandler::operator=() {}

std::vector<std::string> CommandHandler::parse(std::string &message) {
	std::vector<std::string> args;
	args.push_back(message);
	return args;
}

//void CommandHandler::handle(Client *client, std::string &message) {
//	std::stringstream ss(message);
//	std::vector<std::string> args;// = parse(message);
//	std::string buf;
//	while (ss >> buf) {
//		args.push_back(buf);
//		std::cout << buf << "\n";
////		client->addReply(buf);
//	}
//	// loop parse + execute
////	for (ss) {
////		args.push_back();
////	}
////	client->addCommand();
//	//if command not in commands -> error
//	commands["PING"]->execute(client, args);
//}

void CommandHandler::handle(Client *client, std::string &message) {
	std::stringstream ss(message);
	std::string separator;

	while (std::getline(ss, separator)) {
		separator = separator.substr(0, separator[separator.size() - 1] == '\r' ? separator.size() - 1 : separator.size());
		std::string cmd = separator.substr(0, separator.find(' '));

		try {
			Command *command = commands.at(cmd);
			std::vector <std::string> args;
			std::string buf;
			std::stringstream ssin(separator.substr(cmd.size(), separator.size()));

			// todo doesnt work with multiple args
			// ex: JOIN #campfire, #bonfire
			while (ssin >> buf) {
				/// undefined ?
				if (buf[0] == ':')//->++buf
					buf = buf.substr(1);
//				std::cout << buf << "\n";
//				std::cout << buf[0] << "\n";
				args.push_back(buf);
			}
			//if (client->getState() != DONE)
			//	client->getHelp();
			if (command->authRequired() && !client->isRegistered()) {
				client->addReply(serv_host, ERR_NOTREGISTERED());
				return;
			}
//            if (client->getState() == PASSWORD && cmd != "PASS") {
//                client->addReply(serv_host, RPL_USAGE(std::string("use PASS command")));
//                return;
//            }
			/*
            if (client->getState() == USERNAME && cmd != "USER") {
                client->addReply(serv_host, RPL_USAGE(std::string("use USER command")));
                return;
            }
            if (client->getState() == NICKNAME && cmd != "NICK") {
                client->addReply(serv_host, RPL_USAGE(std::string("use NICK command")));
                return;
            }*/

			for (int i = 0; i < args.size(); ++i)
				logger::debug(SSTR("arg[" << i << "]: " <<args[i]));

			command->execute(client, args);
		} catch (const std::out_of_range &e) {
			client->addReply(serv_host, ERR_UNKNOWNCOMMAND(cmd));
		}
	}
}
