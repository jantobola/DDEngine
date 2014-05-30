#pragma once

#include "Skybox.h"
#include "ConstantBuffers.h"
#include "DDEngine.h"

namespace DDEngine
{

class RenderableSkybox : public IRenderable {

private:
	std::string path;
	Skybox skybox;
	CB::WVP_CB vsCB_0;

public:
	RenderableSkybox(DDERenderPackage pkg) : IRenderable(pkg) { }
	~RenderableSkybox();

	virtual void create() override;
	virtual void render() override;

	void setSkyboxPath(std::string path);
	ShaderResourceView* getSkyboxTexture() { return skybox.getSkyboxTexture(); }

};
}

