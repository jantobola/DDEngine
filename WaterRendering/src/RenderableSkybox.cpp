#include "RenderableSkybox.h"

using namespace DDEngine;

RenderableSkybox::~RenderableSkybox() {
	skybox.releaseBuffers();
}

void RenderableSkybox::create() {
	skybox.setDDSTexturePath(path);
	skybox.setShaders("VS_EnvMapping", "PS_EnvMapping", "POS3_TEX");
	skybox.registerObject(Ctx.device, Ctx.context);
}

void RenderableSkybox::render() {
	resources.assignResources(skybox);
	XMStoreFloat4x4(&vsCB_0.WVP, skybox.getSkyboxWVP(camera));
	shaders.updateConstantBufferVS("CB_WVP", &vsCB_0, 0);

	skybox.draw();
	Ctx.setRasterizerState(Ctx.currentRasterizerState);
}

void RenderableSkybox::setSkyboxPath( std::string path ) {
	this->path = path;
}
