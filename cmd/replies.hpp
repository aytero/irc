#ifndef IRC_REPLIES_HPP
#define IRC_REPLIES_HPP

# define ERR_ALREADYREGISTERED(nickname) nickname + " :Unauthorized command (already registered)"
# define ERR_NONICKNAMEGIVEN() ":No nickname given"
# define ERR_ERRONEUSNICKNAME(nick) nick + " :Erroneous nickname"
//:irc.example.com 433 * borja :Nickname is already in use.
# define ERR_NICKNAMEINUSE(nick) nick + " :Nickname is already in use"
//# define ERR_NICKNAMEINUSE(serv, nick) serv + "443 * " + nick + " :Nickname is already in use"

# define ERR_NEEDMOREPARAMS(command) command + " :Not enough parameters"
# define ERR_NOSUCHCHANNEL(channel) channel + " :No such channel"
# define ERR_NOTONCHANNEL(channel) channel + " :You are not on that channel"

# define ERR_TOOMANYCHANNELS(channel) channel name + " :You have joined too many channels"
# define ERR_CHANNELISFULL(channel) channel + " :Cannot join channel (+l)"
# define ERR_BANNEDFROMCHAN(channel) channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(channel, key) channel + " :Cannot join channel (+k)"
# define ERR_NOTREGISTERED() ":You have not registered"
# define ERR_UNKNOWNCOMMAND(command) command + " :Unknown command"

//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu
# define RPL_WELCOME(prefix) ":Welcome to the Internet Replay Network " + prefix
//# define RPL_WELCOME(prefix) ":Welcome to the Internet Replay Network " + prefix
#endif //IRC_REPLIES_HPP
