#pragma once

#include "ShaderHolder.h"

namespace DDEngine
{

class RenderContext;
class Config;
class AbstractObject;

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
		void assignResources(AbstractObject& object);
		void assignResources(AbstractObject& object, unsigned int resIndex);
};
}