#pragma once

#include "DDERenderPackage.h"
#include "ResourceProvider.h"

namespace DDEngine
{
class IRenderable {

	private:

		IRenderable();
		std::string name;

	public:
		
		IRenderable(DDERenderPackage pkg)
			: Ctx(pkg.renderContext),
			resources(pkg.resources),
			shaders(pkg.resources.getShaderHolder()),
			hud(pkg.hud),
			timer(pkg.timer),
			camera(pkg.camera),
			config(pkg.config)
		{ }
		
		virtual ~IRenderable() = 0 { }
	
		void setName(std::string name) { this->name = name; }
		std::string getName() { return name; }

		virtual void create() = 0;
		virtual void render() = 0;
	
	protected:

		RenderContext& Ctx;
		ResourceProvider& resources;
		ShaderHolder& shaders;
		HUDRenderer& hud;
		Timer& timer;
		Camera& camera;
		Config& config;

};
}