#include "Utils.hpp"


std::string utils::vect_to_string( std::vector<std::string> v, unsigned int from) {
	std::string message = "";
	for (; from < v.size() - 1; ++from)
		message.append(v[from] + " ");
	message.append(v[v.size() - 1]);
	return message;
}