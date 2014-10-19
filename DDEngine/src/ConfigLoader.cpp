#include "ConfigLoader.h"
#include <fstream>
#include <algorithm>

using namespace DDEngine;
using namespace std;

ConfigLoader::ConfigLoader() {

}

ConfigLoader::~ConfigLoader() {

}

void ConfigLoader::parseConfig(std::string filename, CFG_SECTION loadSection /*= ALL*/) {
	ifstream input(filename);
	if(!input.is_open()) return;

	CFG_SECTION section;

	for (string line; getline(input, line); ) {

		string trimmed = line;
		trimmed.erase(remove_if(trimmed.begin(), trimmed.end(), isspace), trimmed.end());
		trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\t'), trimmed.end());

		if(trimmed == "") continue;
		if(trimmed.at(0) == CONFIG_COMMENT_CHAR) continue;
		if(trimmed == CONFIG_STARTUP_SECTION) { section = STARTUP; continue; }
		if(trimmed == CONFIG_RENDER_SECTION) { section = RENDER; continue; }

		vector<string> cmdParsed;
	
		switch (section) {

			case STARTUP:
				if (loadSection == ALL || loadSection == STARTUP) {
					CommandParser::parse(&cmdParsed, CONFIG_CMD_DELIMITER, CONFIG_ARG_DELIMITER, line);
					startupCmds.push_back(cmdParsed);
				}
				break;

			case RENDER:
				if (loadSection == ALL || loadSection == RENDER) {
					renderCmds.push_back(line);
				}
				break;
		}
	}

	input.close();
}