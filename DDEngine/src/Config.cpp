#include "Config.h"
#include "DDEUtils.h"
#include <sstream>
#include <algorithm>

using namespace DDEngine;
using namespace std;

Config::Config() : ConfigLoader() {

	path = DDE_CONFIG_PATH;

	CFG_WINDOW_TITLE = L"DDEngine Application";
	CFG_SCREEN_WIDTH = 1024;
	CFG_SCREEN_HEIGHT = 768;
}

Config::~Config() {

}

vector<string> Config::getRenderConfig() {
	return renderCmds;
}

#define ARG(X) cmd.at(X + 1)
#define ARG_BOOL(X) StringUtils::toBool(ARG(X))
#define ARG_FLOAT(X) StringUtils::toFloat(ARG(X))
#define ARG_INT(X) StringUtils::toInt(ARG(X))
#define LOAD(CMD) if(cmd.at(0) == CMD)
#define CONFIG_NO_ARGS if(cmd.size() < 1) continue; 
#define CONFIG_ARGS(X) if(cmd.size() < (X + 1)) continue;
#define NEXT continue;

void Config::load(CFG_SECTION section) {
	parseConfig(path, section);
	for(size_t i = 0; i < startupCmds.size(); i++) {
		vector<string> cmd = startupCmds.at(i);

		if (_delegate) {
			_delegate(cmd);
		}

		CONFIG_ARGS(1)

		LOAD("app.windowTitle")
		{
			CFG_WINDOW_TITLE.assign(ARG(0).begin(), ARG(0).end());			
			NEXT
		}

		LOAD("shaders.defaultPSModel")
		{
			CFG_PIXEL_SHADER_MODEL = ARG(0);
			NEXT
		}

		LOAD("shaders.defaultVSModel")
		{
			CFG_VERTEX_SHADER_MODEL = ARG(0);
			NEXT
		}

		LOAD("shaders.defaultGSModel")
		{
			CFG_GEOMETRY_SHADER_MODEL = ARG(0);
			NEXT
		}

		LOAD("renderer.MSAA")
		{
			MSAA = ARG_INT(0);
			NEXT
		}

		CONFIG_ARGS(2)

		LOAD("app.screenSize")
		{
			CFG_SCREEN_WIDTH = ARG_INT(0);
			CFG_SCREEN_HEIGHT = ARG_INT(1);
			NEXT
		}

		CONFIG_ARGS(3)

		LOAD("[shaders]")
		{
			ShaderConfig shader;
			shader.file = ARG(0);
			shader.entry = ARG(1);
			shader.model = ARG(2);

			stringstream ssLine(shader.file);
			getline(ssLine, shader.name, '.');

			std::size_t found = shader.model.find("vs");
			if (found != std::string::npos) {
				shader.type = ShaderType::VS;
			}

			found = shader.model.find("ps");
			if (found != std::string::npos) {
				shader.type = ShaderType::PS;
			}

			shaders.push_back(shader);

			NEXT
		}

	}
}

Config::string Config::getConfigPath() {
	return path;
}

void Config::setConfigPath( string path ) {
	this->path = path;
}

void DDEngine::Config::delegate(std::function<void(const std::vector<std::string>&)> func) {
	this->_delegate = func;
	load(CFG_SECTION::STARTUP);
}
