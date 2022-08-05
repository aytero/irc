#include "Message.hpp"

Message::Message() {}

Message::Message(const Message &ref) : prefix(ref.prefix), serv_command(ref.serv_command), chan(ref.chan), args(ref.args) {}

Message &Message::operator=(const Message &ref) {
	if (this != &ref) {
		prefix = ref.prefix;
		serv_command = ref.serv_command;
		chan = ref.chan;
		args = ref.args;
	}
	return *this;
}



//Message::Message(const char *s) {}

Message::Message(std::string &s) : prefix(""), serv_command(""), chan("") {
	parse(s);
}

Message::~Message() {}

//std::string &Message::split(std::string &s, std::string delim) {
//
//}

void Message::parse(std::string &s) {

	if (s[s.size() - 2] == '\r')
		s.erase(s.size() - 2);
	else
		s.erase(s.size() - 1);
	//		s.pop_back();

	logger::warn("parser");
	std::queue<std::string> que;

	size_t pos = s.find(' ');
	while (pos != std::string::npos) {
		que.push(s.substr(0, pos));
		s.erase(0, pos + 1);
		pos = s.find(' ');
	}
	if (!s.empty())
		que.push(s);

	logger::warn("splited");


	if (!que.empty() && que.front()[0] == ':') {
		prefix = que.front().substr(1);
		que.pop();
	}
	if (!que.empty()) {
		serv_command = que.front();
		que.pop();
	}
	if (!que.empty() && (que.front()[0] == '#' || que.front()[0] == '&')) {
		chan = que.front();
		que.pop();
	}

	logger::warn("b4 parser loop");
	while (!que.empty()) {
		// make one string with spaces
		if (que.front()[0] == ':') {//->++str
			que.front() = que.front().substr(1);
//			que.pop();
//			std::string temp(que.front().begin() + 1, que.front().end());
//			que.pop();
//			while (!que.empty()) {
////				temp.append(" ");
//				temp.push_back(' ');
//				temp.append(que.front());
//			}
//			args.push_back(temp);
		}
//		} else {
			args.push_back(que.front());
			que.pop();
//		}
	}

	for (unsigned int i = 0; i < args.size(); ++i)
		logger::debug(SSTR("arg[" << i << "]: " << args[i]));
	logger::warn("aft parser loop");
}

std::string &Message::getPrefix() {
	return prefix;
}

std::string &Message::getServCommand() {
	return serv_command;
}

std::string &Message::getChannel() {
	return chan;
}

std::vector<std::string> &Message::getArgs() {
	return args;
}
