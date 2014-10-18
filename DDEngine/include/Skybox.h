#pragma once

#include "Object3D.h"

namespace DDEngine
{

class Camera;

class Skybox : public Object3D {

	private:
		
		ID3D11ShaderResourceView* skyboxTexture;
		ID3D11SamplerState* skyboxSampler;
		ID3D11DepthStencilState* skyboxDepth;
		ID3D11RasterizerState* skyboxRasterizer;

		std::string ddsPath;

		void initContext();
		virtual void loadGeometry(std::vector<Mesh>& meshes) override;
		void cleanUp();

	public:
		
		Skybox()
			: Object3D(), 
			skyboxTexture(NULL), 
			skyboxSampler(NULL), 
			skyboxDepth(NULL), 
			skyboxRasterizer(NULL) { }
		
		Skybox(const Skybox &copy) : Object3D(copy) { }
		~Skybox();

		void setDDSTexturePath(std::string texturePath);
		void draw();
		DirectX::XMMATRIX getSkyboxWVP(Camera& camera);
		ID3D11ShaderResourceView* getSkyboxTexture();

};
}