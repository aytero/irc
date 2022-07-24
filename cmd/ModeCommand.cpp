#include "Command.hpp"

ModeCommand::ModeCommand(bool auth, Server *server) : Command(auth, server) {}

/// channel MODE
//ERR_NEEDMOREPARAMS              ERR_KEYSET
//ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
//ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE

//RPL_CHANNELMODEIS
//RPL_BANLIST                     RPL_ENDOFBANLIST
//RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
//RPL_INVITELIST                  RPL_ENDOFINVITELIST
//RPL_UNIQOPIS

//O - give "channel creator" status;
//o - give/take channel operator privilege;
//v - give/take the voice privilege;

//a - toggle the anonymous channel flag;
//i - toggle the invite-only channel flag;
//m - toggle the moderated channel;
//n - toggle the no messages to channel from clients on the outside;
//q - toggle the quiet channel flag;
//p - toggle the private channel flag;
//s - toggle the secret channel flag;
//r - toggle the server reop channel flag;
//t - toggle the topic settable by channel operator only flag;

//k - set/remove the channel key (password);
//l - set/remove the user limit to channel;

//b - set/remove ban mask to keep users out;
//e - set/remove an exception mask to override a ban mask;
//I - set/remove an invitation mask to automatically override the invite-only flag;
void ModeCommand::chanMode(Client *client, std::vector <std::string> args) {

	std::string &chan_name = args[0];
	Channel *channel = client->getChannel(chan_name);

//	if (!channel) {
//		client->addReply(server_->getHostname(), ERR_NOTONCHANNEL(chan_name));
//		return;
//	}

	if (!channel->isOp(client)) {
		client->addReply(server_->getHostname(), ERR_CHANOPRIVSNEEDED(chan_name));
		return;
	}

	// channel MODE requires more parsing
	// +k "key"
	// +b
	// +b user
	// +b user except_user

//	std::string &target_name = args[1];
	// check if size() > 2
	// of mode flag is about user
//	std::string &target_name = args[2];
//	if (channel->findUser(target_name)) {
//		client->addReply(server_->getHostname(), ERR_USERNOTINCHANNEL(target_name, chan_name));
//		return;
//	}
}


/// user MODE
//ERR_NEEDMOREPARAMS              ERR_USERSDONTMATCH
//ERR_UMODEUNKNOWNFLAG            RPL_UMODEIS

//a - user is flagged as away;
//i - marks a users as invisible;
//w - user receives wallops;
//r - restricted user connection;
//o - operator flag;
//O - local operator flag;
//s - marks a user for receipt of server notices.

//:MODE WiZ -w                    ; turns reception of WALLOPS messages off for WiZ.
//:Angel MODE Angel +i            ; Message from Angel to make themselves invisible.
//MODE WiZ -o
// 									+o or +O  command should be ignored, user must use OPER
void ModeCommand::userMode(Client *client, std::vector <std::string> args) {
	if (args[0] != client->getNickname()) {
		client->addReply(server_->getHostname(), ERR_USERSDONTMATCH());
		return;
	}

	std::string &mode = args[1];
	char modeChar = mode[0];
	bool toggleOn;

	if (modeChar == '+')
		toggleOn = true;
	else if (modeChar == '-')
		toggleOn = false;
	else
		return;
	mode = mode.substr(1);

	/// loop as mode could be "+imI"
	for (int i = 0; i < mode.size(); ++i) {
		modeChar = mode[i];
		if (!client->switchMode(modeChar, toggleOn)) {
			client->addReply(server_->getHostname(), ERR_UMODEUNKNOWNFLAG());
			return;
		}
	}
	client->addReply(server_->getHostname(), RPL_UMODEIS(client->getModeStr()));
}

void ModeCommand::execute(Client *client, std::vector <std::string> args) {
	if (args.empty() || args.size() < 2) {
		client->addReply(server_->getHostname(), ERR_NEEDMOREPARAMS(std::string("MODE")));
		return;
	}
	if (args[0][0] == '#' || args[0][0] == '&')
		chanMode(client, args);
	else
		userMode(client, args);
}