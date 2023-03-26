#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <future>
#include <chrono>
#include <functional>
#include <atomic>

namespace parsers {

	std::string getSplittedStringItem (std::string string, std::string delimiter, int index);
	std::string getFileContents (std::string file_path);


}