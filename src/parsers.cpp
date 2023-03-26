#include "parsers.h"

namespace parsers {

	std::vector<std::string> split(std::string text, char delim) {
		std::string line;
		std::vector<std::string> vec;
		std::stringstream ss(text);
		while (std::getline(ss, line, delim)) {
			vec.push_back(line);
		}
		return vec;
	}

	std::string getSplittedStringItem (std::string string, std::string delimiter, int index) {

		std::vector<std::string> items = parsers::split(string, '~');

		return items[index];
	}
}