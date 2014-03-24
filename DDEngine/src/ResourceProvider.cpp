#include "ResourceProvider.h"
#include "Object3D.h"

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

void ResourceProvider::assignResources(Object3D& object) {
	if(shaderHolder.getActiveVS() != object.getVSName()) shaderHolder.activateVS(object.getVSName());
	if(shaderHolder.getActivePS() != object.getPSName()) shaderHolder.activatePS(object.getPSName());
	if(shaderHolder.getActiveIL() != object.getILName()) shaderHolder.activateIL(object.getILName());
}

ShaderHolder& ResourceProvider::getShaderHolder() {
	return shaderHolder;
}

void ResourceProvider::cleanUp() {
	shaderHolder.cleanUp();
}
