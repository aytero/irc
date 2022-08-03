#ifndef IRC_REPLIES_HPP
#define IRC_REPLIES_HPP

# define ERR_NORECIPIENT(nick, cmd) "411 " + nick + " :No recipient given (" + cmd + ")"
# define ERR_NOTEXTTOSEND(nick) "412 " + nick + " :No text to send"
# define ERR_ALREADYREGISTERED(nickname) "462 " + nickname + " :Unauthorized command (already registered)"
# define ERR_NONICKNAMEGIVEN() "431 * :No nickname given"
# define ERR_ERRONEUSNICKNAME(nick) "432 " + nick + " :Erroneous nickname"
//:irc.example.com 433 * borja :Nickname is already in use.
# define ERR_NICKNAMEINUSE(nick) "433 " + nick + " :Nickname is already in use"
//# define ERR_NICKNAMEINUSE(serv, nick) serv + "443 * " + nick + " :Nickname is already in use"
# define ERR_NEEDMOREPARAMS(command) "461 * " + command + " :Not enough parameters"
# define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"
# define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You are not on that channel"
# define ERR_USERNOTINCHANNEL(nick, channel) "441 * " + nick + " " + channel + " :They aren't on that channel"

# define ERR_TOOMANYCHANNELS(channel) "405 * " + channel + " :You have joined too many channels"
# define ERR_CHANNELISFULL(channel) "471 * " + channel + " :Cannot join channel (+l)"
# define ERR_BANNEDFROMCHAN(channel) "474 * " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(channel, key) "475 * " + channel + " :Cannot join channel (+k)"
# define ERR_NOCHANMODES(channel) "477 " + channel + " :Channel doesn't support modes"
# define ERR_CHANOPRIVSNEEDED(channel) "482 * " + channel + " :You're not channel operator"

# define ERR_NOTREGISTERED() "451 * :You have not registered"
# define ERR_PASSWDMISMATCH() "464 * :Password incorrect"
# define ERR_UNKNOWNCOMMAND(command) "421 * " + command + " :Unknown command"
# define ERR_CANNOTSENDTOCHAN(channel) "404 " + channel + " :Cannot send to channel"
# define ERR_NOSUCHNICK(nick) "401 * " + nick + " :No such nick/channel"
# define ERR_NOSUCHSERVER(server) server + " :No such server" // 402
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
# define RPL_PING(from) "PING :" + from
// PONG
# define RPL_PONG(from, to) "PONG " + from + " " + to
# define RPL_QUIT(user, reason) "QUIT :" + user + " :" + reason
//# define RPL_QUIT(user, reason) "QUIT " + user + ": " + reason
# define RPL_JOIN(user, channel) ":" + user + " JOIN " + channel
# define RPL_KILL(user, reason) "KILL " + user + " :" + reason
# define RPL_MODE(user, mode) user + " MODE +" + mode
# define RPL_LIST_HINT() "channel name | users | topic"
# define RPL_USAGE(step) "Registration steps:\n\t\tPASS <server password>\n\t\tUSER <username> * * :<realname>\n\t\tNICK <nickname>\n\t\t" + step

//:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu
# define RPL_WELCOME(nick, prefix) "001 " + nick + " :Welcome to the Internet Replay Network " + prefix
# define RPL_YOURHOST(nick, host, ver) "002 " + nick + " :Your host is " + host + ", running version " + ver
# define RPL_CREATED(nick, date) "003 " + nick + " :This server was created " + date
# define RPL_MYINFO(nick, servername, version, userModes, chanModes) "004 " + nick + ":" + servername + " " + version + " " + userModes + " " + chanModes

# define RPL_LUSERCLIENT(nick, int1, int2, int3)  "251 " + nick + " :There are " + int1 + " users and " + int2 + " services on " + int3 + " servers"
# define RPL_LUSEROP(nick, int1) "252 "  + nick + " " + int1 + " :operator(s) online"
# define RPL_LUSERUNKNOWN(nick, int1) "253 " + nick + " " + int1 + " :unknown connection(s)"
# define RPL_LUSERCHANNELS(nick, int1) "254 " + nick + " " + int1 + " :channels formed"

# define RPL_LIST(channel, visible, topic) channel + " " + visible + " :" + topic // 322
# define RPL_LISTEND(nick) "323 " + nick + " :End of LIST" // 323

# define RPL_NOTOPIC(nick, channel) "331 " + nick + " " + channel + " :No topic is set"
# define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " :" + topic

# define RPL_UMODEIS(nick, user_mode) "221 " + nick + " " + user_mode
# define RPL_YOUREOPER(nick) "381 " + nick + " :You are now an IRC operator"

# define RPL_MOTDSTART(nick, server) "375 " + nick + " :- " + server + " Message of the day - "
# define RPL_MOTD(nick, text) "372 " + nick + " :- " + text
# define RPL_ENDOFMOTD(nick) "376 " + nick + " :End of MOTD command"
# define ERR_NOMOTD(nick) "422 " + nick + " :MOTD File is missing"

#endif //IRC_REPLIES_HPP
