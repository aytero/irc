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
# define ERR_USERNOTINCHANNEL(nick, channel) "441 * " + nick + " " + channel + " :They aren't on that channel"

# define ERR_TOOMANYCHANNELS(channel) "405 * " + channel + " :You have joined too many channels"
# define ERR_CHANNELISFULL(channel) "471 * " + channel + " :Cannot join channel (+l)"
# define ERR_BANNEDFROMCHAN(channel) "474 * " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(channel, key) "475 * " + channel + " :Cannot join channel (+k)"
# define ERR_NOCHANMODES(channel) "477 " + channel + " :Channel doesn't support modes"
# define ERR_CHANOPRIVSNEEDED(channel) "482 * " + channel + " :You're not channel operator"

# define ERR_NOTREGISTERED() "451 :You have not registered"
# define ERR_PASSWDMISMATCH() "464 :Password incorrect"
# define ERR_UNKNOWNCOMMAND(command) "421 * " + command + " :Unknown command"
# define ERR_NOTEXTTOSEND() "412 :No text to send"
# define ERR_CANNOTSENDTOCHAN(channel) "404" + channel + " :Cannot send to channel"
# define ERR_NOSUCHNICK(nick) "401 * " + nick + " :No such nick/channel"
# define ERR_NOORIGIN() "409 :No origin specified"
# define ERR_NOPRIVILEGES() "481 :Permission Denied - You're not an IRC operator"
# define ERR_NOOPERHOST() "491 :No O-lines for your host"
# define ERR_UMODEUNKNOWNFLAG() "501 :Unknown MODE flag"
# define ERR_USERSDONTMATCH() "502 :Cannot change mode for other users"

/*   Custom rpls   */
# define RPL_PRIVMSG(from, target, message) ":" + from + " PRIVMSG " + target + " :" + message
# define RPL_NOTICE(from, target, message) ":" + from + " NOTICE " + target + " :" + message
# define RPL_KICK(from, channel, target, message) ":" + from + " KICK " + channel + " " + target + " :" + message
# define RPL_PART(from, channel, reason) ":" + from + " PART " + channel + " :" + reason
# define RPL_PING(from)  "PING :" + from
# define RPL_QUIT(user, reason) "QUIT :" + user + " :" + reason
//# define RPL_QUIT(user, reason) "QUIT " + user + ": " + reason
# define RPL_JOIN(user, channel) ":" + user + " JOIN " + channel
# define RPL_KILL(user, reason) "KILL " + user + " :" + reason
# define RPL_MODE(user, mode) user + " MODE +" + mode
# define RPL_LIST_HINT() "channel name | users | topic"

//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu
# define RPL_WELCOME(nick, prefix) "001 " + nick + " :Welcome to the Internet Replay Network " + prefix
# define RPL_YOURHOST(nick, host, ver) "002 " + nick " :Your host is " + host + ", running version " + ver
# define RPL_CREATED(nick, date) "003 " + nick + " :This server was created " + date; }
# define RPL_MYINFO(nick, servername, version, userModes, chanModes) "004 " + nick + ":" + servername + " " + version + " " + userModes + " " + chanModes

//251    RPL_LUSERCLIENT
# define RPL_LUSERCLIENT(int1, int2, int3) ":There are " + int1 + " users and " + int2 + " services on " + int3 + " servers"
//252    RPL_LUSEROP
# define RPL_LUSEROP(int1) int1 + " :operator(s) online"
//253    RPL_LUSERUNKNOWN
# define RPL_LUSERUNKNOWN(int1) int1 + " :unknown connection(s)"
//254    RPL_LUSERCHANNELS
# define RPL_LUSERCHANNELS(int1) int1 + " :channels formed"

# define RPL_LIST(channel, visible, topic) channel + " " + visible + " :" + topic // 322
# define RPL_LISTEND() " :End of LIST" // 323

# define RPL_NOTOPIC(channel) channel + " :No topic is set"
//332    RPL_TOPIC
# define RPL_TOPIC(channel, topic) channel + " :" + topic

//221    RPL_UMODEIS
# define RPL_UMODEIS(user_mode) user_mode
// 381
# define RPL_YOUREOPER() "381 :You are now an IRC operator"
#endif //IRC_REPLIES_HPP

//375    RPL_MOTDSTART
# define RPL_MOTDSTART(server) ":- " + server + " Message of the day - "
//372    RPL_MOTD
# define RPL_MOTD(text) ":- " + text
//376    RPL_ENDOFMOTD
#define RPL_ENDOFMOTD() ":End of MOTD command"
