#include <string>

//std::string rpl_privmsg(const std::string &from, const std::string &target, const std::string &message) {
//	return std::string(":" + from + " PRIVMSG " + target + " :" + message);
//}
//
//std::string Server::err_reply()
//
//std::string err_needmoreparams(const std::string &nick, const std::string &command) {
//	return std::string("461 " nick + " " + command + " :Not enough parameters");
//}

//std::string err_needmoreparams(const std::string &command) { // 461
//	return std::string(command + " :Not enough parameters");
//}


/////// 0xx
// 001    RPL_WELCOME
std::string RPL_WELCOME(std::string prefix) { return ":Welcome to the Internet Replay Network " + prefix; }
//002    RPL_YOURHOST
std::string RPL_YOURHOST(std::string servername, std::string ver) { return ":Your host is " + servername + ", running version " + ver; }
//003    RPL_CREATED
std::string RPL_CREATED(std::string date) { return ":This server was created " + date; }
//004    RPL_MYINFO
std::string RPL_MYINFO(std::string servername, std::string version, std::string userModes, std::string chanModes) {
	return ":" + servername + " " + version + " " + userModes + " " + chanModes; }
//005    RPL_BOUNCE
std::string RPL_BOUNCE(std::string servername, std::string port) { return ":Try server " + servername + ", port " + port; }


/////// 2xx
//200	RPL_TRACELINK
std::string RPL_TRACELINK(std::string version, std::string destination, std::string servername)
	{ return ":Link " + version + " " + destination + " " + servername; }

//201    RPL_TRACECONNECTING
std::string RPL_TRACECONNECTING(std::string class_, std::string server)
	{ return ":Try. " + class_ + " " + server; }

//202    RPL_TRACEHANDSHAKE
std::string RPL_TRACEHANDSHAKE(std::string class_, std::string server)
	{ return ":H.S. " + class_ + " " + server; }

//203    RPL_TRACEUNKNOWN
std::string RPL_TRACEUNKNOWN(std::string class_, std::string clientIp)
	{ return ":???? " + class_ + " " + clientIp; }

//204    RPL_TRACEOPERATOR
std::string RPL_TRACEOPERATOR(std::string class_, std::string nick)
	{ return ":Oper " + class_ + " " + nick; }

//205    RPL_TRACEUSER
std::string RPL_TRACEUSER(std::string class_, std::string nick)
	{ return ":User " + class_ + " " + nick; }

//206    RPL_TRACESERVER
std::string RPL_TRACESERVER(std::string class_, std::string intS, std::string intC, std::string server, std::string nick,
							std::string user, std::string host) {
	return ":Serv " + class_ + " " + intS + "S " + intC + "C " + server + " " + nick + "!" + user + "@ " + host;
}
//207    RPL_TRACESERVICE
std::string RPL_TRACESERVICE(std::string class_, std::string name, std::string type, std::string activeType) {
	return ":Service " + class_ + " " + name + " " + type + " " + activeType;
}

//208    RPL_TRACENEWTYPE
std::string RPL_TRACENEWTYPE(std::string newType, std::string clientName) { return ": " + newType + " 0 " + clientName; }

//209    RPL_TRACECLASS
std::string RPL_TRACECLASS(std::string class_, std::string count) { return ":Class " + class_ + " " + count; }

//210    RPL_TRACERECONNECT
//              Unused.

//261    RPL_TRACELOG
std::string RPL_TRACELOG(std::string logfile, std::string debug_level) { return ":File " + logfile + " " + debug_level; }

//262    RPL_TRACEEND
std::string RPL_TRACEEND(std::string servername, std::string version) { return servername + " " + version + " :End of TRACE"; }

// 211    RPL_STATSLINKINFO
std::string RPL_STATSLINKINFO(std::string linkname, std::string sendq, std::string sent_messages, std::string sent_kbytes,
							  std::string recv_messages, std::string recv_kbytes, std::string time_open) {
	return linkname + " " + sendq + " " + sent_messages + " " + sent_kbytes + " " + recv_messages + " "
			+ recv_kbytes + " " + time_open;
}

// 212    RPL_STATSCOMMANDS
std::string RPL_STATSCOMMANDS(std::string command, std::string count, std::string byte_count, std::string remote_count) {
	return command + " " + count + " " + byte_count + " " + remote_count;
}

//219    RPL_ENDOFSTATS
std::string RPL_ENDOFSTATS(std::string status_letter) { return status_letter + " :End of STATS report"; }

//242    RPL_STATSUPTIME
std::string RPL_STATSUPTIME() { return ":Server Up %d days %d:%02d:%02d"; }

//243    RPL_STATSOLINE
std::string RPL_STATSOLINE(std::string hostmask, std::string name) { return "O " + hostmask + " * " + name; }

//221    RPL_UMODEIS
std::string RPL_UMODEIS(std::string user_mode) { return user_mode; }

//234    RPL_SERVLIST
//std::string RPL_SERVLIST(std::string user_mode) { return user_mode; }
//       "<name> <server> <mask> <type> <hopcount> <info>"

//235    RPL_SERVLISTEND
std::string RPL_SERVLISTEND(std::string mask, std::string type) { return mask + " " + type + " :End of service listing"; }

//251    RPL_LUSERCLIENT
std::string RPL_LUSERCLIENT(std::string int1, std::string int2, std::string int3) {
	return ":There are " + int1 + " users and " + int2 + " services on " + int3 + " servers";
}
//252    RPL_LUSEROP
std::string RPL_LUSEROP(std::string int1) { return int1 + " :operator(s) online"; }

//253    RPL_LUSERUNKNOWN
std::string RPL_LUSERUNKNOWN(std::string int1) { return int1 + " :unknown connection(s)"; }

//254    RPL_LUSERCHANNELS
std::string RPL_LUSERCHANNELS(std::string int1) { return int1 + " :channels formed"; }

//255    RPL_LUSERME
std::string RPL_LUSERME(std::string int1, std::string int2) { return ":I have " + int1 + " clients and " + int2 + " servers"; }

//256    RPL_ADMINME
std::string RPL_ADMINME(std::string server) { return server + " c:Administrative info"; }

//257    RPL_ADMINLOC1
std::string RPL_ADMINLOC1(std::string admin_info) { return ":" + admin_info; }

//258    RPL_ADMINLOC2
std::string RPL_ADMINLOC2(std::string admin_info) { return ":" + admin_info; }

//259    RPL_ADMINEMAIL
std::string RPL_ADMINEMAIL(std::string admin_info) { return ":" + admin_info; }

//263    RPL_TRYAGAIN
std::string RPL_TRYAGAIN(std::string command) { return command + " :Please wait a while and try again."; }


////////// 3xx
//302    RPL_USERHOST
std::string RPL_USERHOST(std::string reply_list) { return  ":" + reply_list; }
//303    RPL_ISON
std::string RPL_ISON(std::string nick_list) { return  ":" + nick_list; }
//301    RPL_AWAY
std::string RPL_AWAY(std::string nick, std::string away_msg) { return  nick + " :" + away_msg; }
//305    RPL_UNAWAY
std::string RPL_UNAWAY() { return  ":You are no longer marked as being away"; }
//306    RPL_NOWAWAY
std::string RPL_NOWAWAY() { return  ":You have been marked as being away"; }

//311    RPL_WHOISUSER
std::string RPL_WHOISUSER(std::string nick, std::string user, std::string host, std::string real_name) {
	return nick + " " + user + " " + host + " * :" + real_name;
}
//312    RPL_WHOISSERVER
std::string RPL_WHOISSERVER(std::string nick, std::string server, std::string server_info) { return nick + " " + server + " :" + server_info; }
//313    RPL_WHOISOPERATOR
std::string RPL_WHOISOPERATOR(std::string nick) { return nick + " :is an IRC operator"; }
//317    RPL_WHOISIDLE
std::string RPL_WHOISIDLE(std::string nick, std::string integer) { return nick + " " + integer + " :seconds idle"; }
//318    RPL_ENDOFWHOIS
std::string RPL_ENDOFWHOIS(std::string nick) { return nick + " :End of WHOIS list"; }
//319    RPL_WHOISCHANNELS
std::string RPL_WHOISCHANNELS(std::string nick, std::string channel) { return nick + " :" + channel; }
//314    RPL_WHOWASUSER
std::string RPL_WHOWASUSER(std::string nick, std::string user, std::string host, std::string real_name) {
	return nick + " " + user + " " + host + " * :" + real_name;
}
//369    RPL_ENDOFWHOWAS
std::string RPL_ENDOFWHOWAS(std::string nick) { return nick + " :End of WHOWAS"; }
//321    RPL_LISTSTART
//       Obsolete. Not used.
//322    RPL_LIST
std::string RPL_LIST(std::string channel, std::string visible, std::string topic) { return channel + " " + visible + " :" + topic; }
//323    RPL_LISTEND
std::string RPL_LISTEND() { return " :End of LIST"; }
//325    RPL_UNIQOPIS
std::string RPL_UNIQOPIS(std::string channel, std::string nick) { return channel + " " + nick; }
//324    RPL_CHANNELMODEIS
std::string RPL_CHANNELMODEIS(std::string channel, std::string mode, std::string mode_params) { return channel + " " + mode + " " + mode_params; }
//331    RPL_NOTOPIC
std::string RPL_NOTOPIC(std::string channel) { return channel + " :No topic is set"; }
//332    RPL_TOPIC
std::string RPL_TOPIC(std::string channel, std::string topic) { return channel + " :" + topic; }
//341    RPL_INVITING
std::string RPL_INVITING(std::string channel, std::string nick) { return nick + " " + channel; }
//342    RPL_SUMMONING
std::string RPL_SUMMONING(std::string user) { return user + " :Summoning user to IRC"; }
//346    RPL_INVITELIST
std::string RPL_INVITELIST(std::string channel, std::string invite_mask) { return channel + " " + invite_mask; }
//347    RPL_ENDOFINVITELIST
std::string RPL_ENDOFINVITELIST(std::string channel) { return channel + " :End of channel invite list"; }
//348    RPL_EXCEPTLIST
std::string RPL_EXCEPTLIST(std::string channel, std::string exception_mask) { return channel + " " + exception_mask; }
//349    RPL_ENDOFEXCEPTLIST
std::string RPL_ENDOFEXCEPTLIST(std::string channel) { return channel + " :End of channel exception list"; }
//351    RPL_VERSION
std::string RPL_VERSION(std::string version, std::string server, std::string comments) { return  version + " " + server + " :" + comments; }
//352    RPL_WHOREPLY
std::string RPL_WHOREPLY(std::string channel, std::string user, std::string host, std::string server, std::string nick,
						 std::string hopcount, std::string real_name) {
	return  channel + " " + user + " " + host + " " + server + " " + nick + " :" + hopcount + " " + real_name;
}
//315    RPL_ENDOFWHO
std::string RPL_ENDOFWHO(std::string name) { return name + " :End of /WHO list"; }
//353    RPL_NAMREPLY
std::string RPL_NAMREPLY(std::string chan_mode, std::string channel, std::string nick_list) { return chan_mode + " " + channel + " :" + nick_list; }
//366    RPL_ENDOFNAMES
std::string RPL_ENDOFNAMES(std::string channel) { return channel + " :End of NAMES list"; }
//364    RPL_LINKS
std::string RPL_LINKS(std::string mask, std::string server, std::string hopcount, std::string server_info) {
	return mask + " " + server + " :" + hopcount + " " + server_info;
}
//365    RPL_ENDOFLINKS
std::string RPL_ENDOFLINKS(std::string mask) { return mask + " :End of LINKS list"; }
//367    RPL_BANLIST
std::string RPL_BANLIST(std::string channel, std::string ban_mask) { return channel + " " + ban_mask; }
//368    RPL_ENDOFBANLIST
std::string RPL_ENDOFBANLIST(std::string channel) { return channel + " :End of channel ban list"; }
//371    RPL_INFO
std::string RPL_ENDOFBANLIST(std::string str) { return ":" + str; }
//374    RPL_ENDOFINFO
std::string RPL_ENDOFINFO() { return ":End of INFO list"; }
//375    RPL_MOTDSTART
std::string RPL_MOTDSTART(std::string server) { return ":- " + server + " Message of the day - "; }
//372    RPL_MOTD
std::string RPL_MOTD(std::string text) { return ":- " + text; }
//376    RPL_ENDOFMOTD
std::string RPL_ENDOFMOTD() { return ":End of MOTD command"; }
//381    RPL_YOUREOPER
std::string RPL_YOUREOPER() { return ":You are now an IRC operator"; }
//382    RPL_REHASHING
std::string RPL_REHASHING(std::string config_file) { return config_file + " :Rehashing"; }
//383    RPL_YOURESERVICE
std::string RPL_YOURESERVICE(std::string service_name) { return "You are service " + service_name; }
//391    RPL_TIME
std::string RPL_TIME(std::string server, std::string local_time) { return server + " :" + local_time; }
//392    RPL_USERSSTART
std::string RPL_USERSTART() { return " :UserID Terminal Host"; }
//393    RPL_USERS
std::string RPL_USERS(std::string username, std::string ttyline, std::string hostname) { return ":" + username + " " + ttyline + " " + hostname; }
//394    RPL_ENDOFUSERS
std::string RPL_ENDOFUSERS() { return ":End of users"; }
//395    RPL_NOUSERS
std::string RPL_NOUSERS() { return ":Nobody logged in"; }


////// 4xx
