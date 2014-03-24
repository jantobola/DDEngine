#pragma once

namespace DDEngine
{

class RenderContext;
class ResourceProvider;
class ShaderHolder;
class HUDRenderer;
class Camera;
class Timer;
class Config;

struct DDERenderPackage {

	DDERenderPackage(

			RenderContext& renderContext,
			ResourceProvider& resources,
			ShaderHolder& shaders,
			HUDRenderer& hud,
			Camera& camera,
			Timer& timer,
			Config& config
			
			) :
			
			renderContext(renderContext),
			resources(resources),
			shaders(shaders),
			hud(hud),
			camera(camera),
			timer(timer), 
			config(config) {
		
	}

	RenderContext& renderContext;
	ResourceProvider& resources;
	ShaderHolder& shaders;
	HUDRenderer& hud;
	Timer& timer;
	Camera& camera;
	Config& config;

};
}