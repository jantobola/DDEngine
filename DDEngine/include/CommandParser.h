#pragma once;

#include <string>
#include <vector>

namespace DDEngine
{
class CommandParser {

	private:
		CommandParser () { }
		~CommandParser () { }

	public:
		static void parse(std::vector<std::string>* store, 
						  char delimiter, char argDelimiter, std::string line);

};
}