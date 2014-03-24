#pragma once

#include "CommandParser.h"

#define CONFIG_STARTUP_SECTION ">startup"
#define CONFIG_RENDER_SECTION ">render"
#define CONFIG_CMD_DELIMITER '='
#define CONFIG_COMMENT_CHAR '#'

namespace DDEngine
{
class ConfigLoader {
	
	private:
		enum CFG_SECTION
		{
			STARTUP,
			RENDER,
		};

	protected:
		std::vector< std::vector< std::string >> startupCmds;
		std::vector< std::string > renderCmds;
		
	public:
		ConfigLoader();
		~ConfigLoader();

		void parseConfig(std::string filename);

};
}