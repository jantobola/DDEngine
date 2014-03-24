#pragma once

#include <string>
#include <unordered_set>

namespace DDEngine
{

class IRenderable;
class RenderContext;
class Config;

class ObjectManager {
	
	private:

		Config& config;
		RenderContext& renderContext;
		std::vector<IRenderable*> renderableObjects;
		std::tr1::unordered_set<std::string> hiddenObjects;

	public:

		ObjectManager(Config& config, RenderContext& renderContext);
		~ObjectManager();

		void addObject(IRenderable* renderableObject);
		void hide(std::string name);
		void show(std::string name);
		
		void create();
		void render();

};
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        