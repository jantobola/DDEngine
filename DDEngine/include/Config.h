#pragma once

#include "ConfigLoader.h"
#include "ShaderHolder.h"
#include <functional>

#define DDE_CONFIG_PATH "config.cfg"

namespace DDEngine
{

	struct ShaderConfig
	{
		std::string name;
		std::string file;
		std::string model;
		std::string entry;
		ShaderType type;
	};

class Config : public ConfigLoader {
	
	typedef std::string string;
	typedef std::wstring wstring;

	private:
		string path;
		std::function<void(const std::vector<std::string>&)> _delegate;

	public:
		Config();
		~Config();

		string getConfigPath();
		void setConfigPath(string path);

		void load(CFG_SECTION section = CFG_SECTION::ALL);
		virtual void delegate(std::function<void(const std::vector<std::string>&)> func);

		std::vector<string> getRenderConfig();

		string CFG_VERTEX_SHADER_MODEL = "vs_4_0";
		string CFG_PIXEL_SHADER_MODEL = "ps_4_0";
		string CFG_GEOMETRY_SHADER_MODEL = "gs_4_0";
		wstring CFG_WINDOW_TITLE;
		int CFG_SCREEN_WIDTH;
		int CFG_SCREEN_HEIGHT;
		int MSAA = 1;
		int AF = 16;

		std::vector<ShaderConfig> shaders;

};
}