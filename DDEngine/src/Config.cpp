#include "Config.h"
#include "DDEUtils.h"

using namespace DDEngine;
using namespace std;

Config::Config() : ConfigLoader() {

	path = DDE_CONFIG_PATH;

	CFG_VERTEX_SHADER_MODEL = "vs_4_0";
	CFG_PIXEL_SHADER_MODEL = "ps_4_0";
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

void Config::load() {
	parseConfig(path);
	for(size_t i = 0; i < startupCmds.size(); i++) {
		vector<string> cmd = startupCmds.at(i);

		CONFIG_ARGS(1)

		LOAD("window_title")
		{
			CFG_WINDOW_TITLE.assign(ARG(0).begin(), ARG(0).end());			
			NEXT
		}

		LOAD("pixel_shader_model")
		{
			CFG_PIXEL_SHADER_MODEL = ARG(0);
			NEXT
		}

		LOAD("vertex_shader_model")
		{
			CFG_VERTEX_SHADER_MODEL = ARG(0);
			NEXT
		}

		LOAD("MSAA")
		{
			MSAA = ARG_INT(0);
			NEXT
		}

		LOAD("AF")
		{
			AF = ARG_INT(0);
			NEXT
		}

		LOAD("elevation_factor")
		{
			ELEVATION_FACTOR = ARG_FLOAT(0);
			NEXT
		}

		LOAD("drop_strength")
		{
			DROP_STRENGTH = ARG_FLOAT(0);
			NEXT
		}

		LOAD("viscosity")
		{
			VISCOSITY = ARG_FLOAT(0);
			NEXT
		}

		LOAD("terrain_texture_scale")
		{
			TERRAIN_TEXTURE_SCALE = ARG_FLOAT(0);
			NEXT
		}

		CONFIG_ARGS(2)

		LOAD("screen_resolution")
		{
			CFG_SCREEN_WIDTH = ARG_INT(0);
			CFG_SCREEN_HEIGHT = ARG_INT(1);
			NEXT
		}

		LOAD("water_grid")
		{
			WATER_GRID_X = ARG_INT(0);
			WATER_GRID_Y = ARG_INT(1);
			NEXT
		}

		LOAD("terrain_grid")
		{
			TERRAIN_GRID_X = ARG_INT(0);
			TERRAIN_GRID_Y = ARG_INT(1);
			NEXT
		}

		CONFIG_ARGS(3)
		
		LOAD("light_dir")
		{
			LIGHT_DIR_X = ARG_FLOAT(0);
			LIGHT_DIR_Y = ARG_FLOAT(1);
			LIGHT_DIR_Z = ARG_FLOAT(2);
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
