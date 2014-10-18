#pragma once

#include "ShaderHolder.h"

namespace DDEngine
{

class RenderContext;
class Config;
class Object3D;

class ResourceProvider {

	private:
		ShaderHolder shaderHolder;
		Config& config;
		RenderContext& renderContext;

		void cleanUp();

	public:
		ResourceProvider(Config& config, RenderContext& renderContext);
		~ResourceProvider();

		void load();

		ShaderHolder& getShaderHolder();
		void assignResources(Object3D& object);
		void assignResources(Object3D& object, unsigned int resIndex);
};
}