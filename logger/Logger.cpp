#include "Logger.hpp"

void	logger::setLogLevel(LogLevel level) {
	logLevel = level;
}

logger::LogLevel	logger::getLogLevel() {
	return logLevel;
}

void	logger::print_time() {
	std::cout << GRN << '[' << get_timestr() << "] implement it yourself if you want to"
		<< YEL << " (= ФェФ=)" << RESET << '\n';
}

std::string	logger::get_timestr() {
	static char	buffer[30];
	std::time_t	t = std::time(nullptr);

	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&t));
	return std::string(buffer);
}

void logger::error(const std::string &message) {
	if (getLogLevel() >= ERROR) {
		std::cerr << RED << '[' << get_timestr() << "] " << message << RESET << '\n';
	}
}

void logger::warn(const std::string &message) {
	if (getLogLevel() >= WARNING) {
		std::cerr << YEL << '[' << get_timestr() << "] " << message << RESET << '\n';
	}
}

void logger::info(const std::string &message) {
	if (getLogLevel() >= INFO) {
		std::cout << GRN << '[' << get_timestr() << "] " << message << RESET << '\n';
	}
}

void logger::debug(const std::string &message) {
	if (getLogLevel() >= DEBUG) {
		std::cout << GREY << '[' << get_timestr() << "] " << message << RESET << '\n';
	}
}

//template<class T>
//void logger::log( int type, typename enable_if<is_integral<T>::value, T>::type &msg) {
//	if (type == DEBUG) {
//		std::cout << GREY"[DEBUG] " << msg << RESET << std::endl;
//	} else if (type == INFO) {
//		std::cout << GRN"[INFO] " << msg << RESET << std::endl;
//	} else if (type == WARNING) {
//		std::cout << YEL"[WARNING] " << msg << RESET << std::endl;
//	} else if (type == ERROR) {
//		std::cout << RED"[ERROR] " << msg << RESET << std::endl;
//	}
//}
