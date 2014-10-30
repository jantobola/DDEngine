#include "ResourceProvider.h"
#include "AbstractObject.h"

using namespace DDEngine;

ResourceProvider::ResourceProvider(Config& config, RenderContext& renderContext) : 
	config(config), 
	renderContext(renderContext), 
	shaderHolder(config, renderContext) {

}

ResourceProvider::~ResourceProvider() {
	cleanUp();
}

void ResourceProvider::load() {
	shaderHolder.load();
}

void DDEngine::ResourceProvider::assignResources(AbstractObject& object, unsigned int resIndex)
{
	if (shaderHolder.getActiveVS() != object.getShaders()[resIndex].vsName) shaderHolder.activateVS(object.getShaders()[resIndex].vsName);
	if (shaderHolder.getActivePS() != object.getShaders()[resIndex].psName) shaderHolder.activatePS(object.getShaders()[resIndex].psName);
	if (shaderHolder.getActiveIL() != object.getShaders()[resIndex].ilName) shaderHolder.activateIL(object.getShaders()[resIndex].ilName);
}

void DDEngine::ResourceProvider::assignResources(AbstractObject& object)
{
	if (object.getShaders().size() > 0) {
		assignResources(object, 0);
	}
}

ShaderHolder& ResourceProvider::getShaderHolder() {
	return shaderHolder;
}

void ResourceProvider::cleanUp() {
	shaderHolder.cleanUp();
}
