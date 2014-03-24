#include "ObjectManager.h"
#include "IRenderable.h"
#include "Config.h"
#include "RenderContext.h"

using namespace DDEngine;
using namespace std;

ObjectManager::ObjectManager(Config& config, RenderContext& renderContext) : config(config), renderContext(renderContext) {

}

ObjectManager::~ObjectManager() {
	
}

void ObjectManager::addObject(IRenderable* renderableObject) {
	renderableObjects.push_back(renderableObject);
}

void ObjectManager::create() {
	for(size_t i = 0; i < renderableObjects.size(); i++) {
		IRenderable* object = renderableObjects.at(i);
		if(object) object->create();
	}
}

void ObjectManager::render() {
	for(size_t i = 0; i < renderableObjects.size(); i++) {
		IRenderable* object = renderableObjects.at(i);
		
		if(object) {
		
			if(!hiddenObjects.count(object->getName())) {
				object->render();	
			}
		}
	}
}

void ObjectManager::hide( std::string name ) {
	hiddenObjects.insert(name);
}

void ObjectManager::show( std::string name ) {
	hiddenObjects.erase(name);
}