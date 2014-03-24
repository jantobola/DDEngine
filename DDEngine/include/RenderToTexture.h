#pragma once

#include "QuadObject.h"
#include <d3d11.h>
#include <windows.h>

namespace DDEngine
{

class RenderToTexture {

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* context;

		ID3D11Texture2D* texture2D;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11ShaderResourceView* shaderResourceView;

		QuadObject quad;

		void cleanUp();

	public:
		RenderToTexture();
		RenderToTexture(ID3D11Device* device, ID3D11DeviceContext* context);
		~RenderToTexture();
	
		HRESULT create(int width, int height);
		ID3D11ShaderResourceView* getShaderResourceView();
		ID3D11RenderTargetView* getRenderTargetView();

		QuadObject& getQuad();
		void setShaders(std::string vs, std::string ps, std::string il);
		void draw();
		
};
}