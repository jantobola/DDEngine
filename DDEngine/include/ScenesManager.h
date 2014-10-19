#pragma once

#include <string>
#include <unordered_set>

namespace DDEngine
{

class IRenderable;
class RenderContext;
class Config;

class ScenesManager {
	
	private:

		Config& config;
		RenderContext& renderContext;
		std::vector<IRenderable*> renderableScenes;
		std::tr1::unordered_set<std::string> hiddenObjects;

	public:

		ScenesManager(Config& config, RenderContext& renderContext);
		~ScenesManager();

		void addScene(IRenderable* renderableScene);
		void hide(const std::string& name);
		void show(const std::string& name);
		
		void create();
		void render();

};
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        