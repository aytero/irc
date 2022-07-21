#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <ctime>
# include <sstream>

# define SSTR( x ) static_cast<std::ostringstream &>( std::ostringstream() << std::dec << x ).str()

# define GREY "\e[38;5;241m"
# define GRN "\e[38;5;150m"
# define RED "\e[38;5;210m"
# define YEL "\e[38;5;222m"
# define RESET "\033[0m"

namespace logger {

	enum LogLevel {
		SILENT,
		ERROR,
		WARNING,
		INFO,
		DEBUG
	};

	namespace {
		LogLevel	logLevel;
	}

	void		setLogLevel( LogLevel level );
	LogLevel	getLogLevel( void );

	void		print_time( void );

	std::string	get_timestr( void );

	void error( const std::string &message );
	void warn( const std::string &message );
	void info( const std::string &message );
	void debug( const std::string &message );

	template< typename T >
	void	error( T &message ) {
		if (getLogLevel() >= ERROR) {
			std::cerr << RED << '[' << get_timestr() << "] " << message << RESET << '\n';
		}
	}

	template< typename T >
	void	warn( T &message ) {
		if (getLogLevel() >= WARNING) {
			std::cerr << YEL << '[' << get_timestr() << "] " << message << RESET << '\n';
		}
	}

	template< typename T >
	void	info( T &message ) {
		if (getLogLevel() >= INFO) {
			std::cout << GRN << '[' << get_timestr() << "] " << message << RESET << '\n';
		}
	}

	template< typename T >
	void	debug( T &message ) {
		if (getLogLevel() >= DEBUG) {
			std::cout << GREY << '[' << get_timestr() << "] " << message << RESET << '\n';
		}
	}


};


#endif //LOGGER_HPP
