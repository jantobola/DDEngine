#pragma once

#include <DDEngine.h>
#include <Skybox.h>
#include "ConstantBuffers.h"

class RenderableSkybox : public DDEngine::IRenderable {

	private:
		std::string path;
		DDEngine::Skybox skybox;
		WVP_CB vsCB_0;

	public:
		RenderableSkybox(DDEngine::DDERenderPackage pkg) : DDEngine::IRenderable(pkg) { }
		~RenderableSkybox();
	
		virtual void create() override;
		virtual void render() override;

		void setSkyboxPath(std::string path);
		DDEngine::ShaderResourceView* getSkyboxTexture() { return skybox.getSkyboxTexture(); }
		
};