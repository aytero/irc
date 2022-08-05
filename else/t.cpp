#include <iostream>
#include <ctime>

int main() {
//	std::string str = "long response";
//	int len = 6;
//
//	std::cout << (str.substr(len)) << "\n";
//
//	time_t s = 60;
//	std::cout << s << "\n";
//
//	std::string emt;
//	std::cout << emt.empty() << " empty\n";
//	std::cout << emt.size() << " size\n";
//	std::cout << emt << "\n";
//
//	emt = "";
//	std::cout << emt.empty() << " empty\n";
//	std::cout << emt.size() << " size\n";
//	std::cout << emt << "\n";

	std::string er = ":pps.example PRIVMSG #chan :bro what a string!";
	std::cout << er << "\n";
	er.erase(er.size() - 2);
	std::cout << er << "\n";
}

