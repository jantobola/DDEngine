#include "RenderableSkybox.h"

using namespace DDEngine;

RenderableSkybox::~RenderableSkybox() {

}

void RenderableSkybox::create() {
	skybox.setDDSTexturePath(path);
	skybox.addShaderCombination("DDEngine_Skybox", "DDEngine_VS_Skybox", "DDEngine_PS_Skybox", "POS_TEX");
	skybox.registerObject("skybox", Ctx);
}

void RenderableSkybox::render() {
	resources.assignResources(skybox);
	XMStoreFloat4x4(&vsCB_0.WVP, skybox.getSkyboxWVP(camera));
	shaders.updateConstantBufferVS("DDEngine_CB_WVP", &vsCB_0, 0);

	skybox.draw();
	Ctx.setRasterizerState(Ctx.currentRasterizerState);
}

void RenderableSkybox::setSkyboxPath( std::string path ) {
	this->path = path;
}
