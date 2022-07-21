#ifndef IRC_REPLIES_HPP
#define IRC_REPLIES_HPP

# define ERR_ALREADYREGISTERED(nickname) nickname + "462 :Unauthorized command (already registered)"
# define ERR_NONICKNAMEGIVEN() "431 :No nickname given"
# define ERR_ERRONEUSNICKNAME(nick) "432 * " + nick + " :Erroneous nickname"
//:irc.example.com 433 * borja :Nickname is already in use.
# define ERR_NICKNAMEINUSE(nick) "433 * " + nick + " :Nickname is already in use"
//# define ERR_NICKNAMEINUSE(serv, nick) serv + "443 * " + nick + " :Nickname is already in use"
# define ERR_NEEDMOREPARAMS(command) "461 * " + command + " :Not enough parameters"
# define ERR_NOSUCHCHANNEL(channel) "403 * " + channel + " :No such channel"
# define ERR_NOTONCHANNEL(channel) "442 * " + channel + " :You are not on that channel"

# define ERR_TOOMANYCHANNELS(channel) "405 * " + channel + " :You have joined too many channels"
# define ERR_CHANNELISFULL(channel) "471 * " + channel + " :Cannot join channel (+l)"
# define ERR_BANNEDFROMCHAN(channel) "474 * " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(channel, key) "475 * " + channel + " :Cannot join channel (+k)"
# define ERR_NOCHANMODES(channel) "477 " + channel + " :Channel doesn't support modes"
# define ERR_CHANOPRIVSNEEDED(channel) "482 * " + channel + " :You're not channel operator"

# define ERR_NOTREGISTERED() "451 :You have not registered"
# define ERR_UNKNOWNCOMMAND(command) "421 * " + command + " :Unknown command"
# define ERR_NOTEXTTOSEND() "412 :No text to send"
# define ERR_CANNOTSENDTOCHAN(channel) "404" + channel + " :Cannot send to channel"
# define ERR_NOSUCHNICK(nick) "401 * " + nick + " :No such nick/channel"


//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu
# define RPL_WELCOME(nick, prefix) "001 " + nick + " :Welcome to the Internet Replay Network " + prefix
//# define RPL_WELCOME(prefix) ":Welcome to the Internet Replay Network " + prefix
# define RPL_PRIVMSG(from, target, message) ":" + from + " PRIVMSG " + target + " :" + message
# define RPL_NOTICE(from, target, message) ":" + from + " NOTICE " + target + " :" + message

# define RPL_YOURHOST(servername, ver) ":Your host is " + servername + ", running version " + ver
# define RPL_MYINFO(servername, version, userModes, chanModes) ":" + servername + " " + version + " " + userModes + " " + chanModes

# define RPL_NOTOPIC(channel) channel + " :No topic is set"
//332    RPL_TOPIC
# define RPL_TOPIC(channel, topic) channel + " :" + topic

# define RPL_PART(from, channel, reason) ":" + from + " PART " + channel + " :" + reason

#endif //IRC_REPLIES_HPP
