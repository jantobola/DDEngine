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
		int AF = 16;


		// NOT ENGINE RELATED THINGS
		int WATER_GRID_X = 256;
		int WATER_GRID_Y = 256;

		int TERRAIN_GRID_X = 256;
		int TERRAIN_GRID_Y = 256;
		float TERRAIN_TEXTURE_SCALE = 1;

		float ELEVATION_FACTOR = 0.28f;
		float DROP_STRENGTH = 0.0029f;
		float VISCOSITY = 0.9975f;

		float LIGHT_DIR_X = 1;
		float LIGHT_DIR_Y = 1;
		float LIGHT_DIR_Z = 1;

};
}