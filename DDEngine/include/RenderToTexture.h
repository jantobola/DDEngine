#pragma once

#include "QuadObject.h"
#include <d3d11.h>
#include <windows.h>

namespace DDEngine
{

typedef ID3D11ShaderResourceView ShaderResourceView;

class RenderToTexture {

	private:
		RenderContext* Ctx;

		ID3D11Texture2D* texture2D = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;
		ShaderResourceView* shaderResourceView = nullptr;
		ID3D11DepthStencilView*	depthStencilView = nullptr;
		ID3D11Texture2D* depthStencilBuffer = nullptr;

		DXGI_FORMAT format;

		int width = 1;
		int height = 1;

		QuadObject quad;

		void cleanUp();

	public:
		RenderToTexture();
		RenderToTexture(RenderContext& Ctx);
		~RenderToTexture();
	
		HRESULT create(int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);
		HRESULT createDepth();

		ShaderResourceView* getShaderResourceView();
		ID3D11RenderTargetView* getRenderTargetView();
		ID3D11DepthStencilView* getDepthStencilView();

		QuadObject& getQuad();
		void setShaders(std::string vs, std::string ps, std::string il);
		void draw();
		
		int getWidth();
		int getHeight();

};
}