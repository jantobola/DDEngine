#pragma once

#include "ConfigLoader.h"

#define DDE_CONFIG_PATH "config.cfg"

namespace DDEngine
{
class Config : public ConfigLoader {
	
	typedef std::string string;
	typedef std::wstring wstring;

	private:
		string path;

	public:
		Config();
		~Config();

		string getConfigPath();
		void setConfigPath(string path);

		void load();
		std::vector<string> getRenderConfig();

		string CFG_VERTEX_SHADER_MODEL;
		string CFG_PIXEL_SHADER_MODEL;
		wstring CFG_WINDOW_TITLE;
		int CFG_SCREEN_WIDTH;
		int CFG_SCREEN_HEIGHT;
		int MSAA = 1;
};
}