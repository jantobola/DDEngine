#include "ScenesManager.h"
#include "IRenderable.h"
#include "Config.h"
#include "RenderContext.h"

using namespace DDEngine;
using namespace std;

ScenesManager::ScenesManager(Config& config, RenderContext& renderContext) : config(config), renderContext(renderContext) {

}

ScenesManager::~ScenesManager() {
	
}

void ScenesManager::addScene(IRenderable* renderableScene) {
	renderableScenes.push_back(renderableScene);
}

void ScenesManager::create() {

	transformator.setObjects(renderContext.getRegisteredObjects());
	transformator.initBar(config.CFG_SCREEN_WIDTH, config.CFG_SCREEN_HEIGHT);
	transformator.setEnabledTransformations(false);

	for (size_t i = 0; i < renderableScenes.size(); i++) {
		IRenderable* object = renderableScenes.at(i);
		if(object) object->create();
	}
}

void ScenesManager::render(const DDERenderPackage pkg) {

	for (size_t i = 0; i < renderableScenes.size(); i++) {
		IRenderable* object = renderableScenes.at(i);
		
		if(object) {
		
			if(!hiddenObjects.count(object->getName())) {
				object->render();	
			}
		}
	}

	transformator.transform(pkg);
}

void ScenesManager::hide(const std::string& name) {
	hiddenObjects.insert(name);
}

void ScenesManager::show(const std::string& name) {
	hiddenObjects.erase(name);
}