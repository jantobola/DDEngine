#pragma once

#include "QuadObject.h"
#include <d3d11.h>
#include <windows.h>

namespace DDEngine
{

typedef ID3D11ShaderResourceView ShaderResourceView;

class RenderToTexture {

	private:
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;

		ID3D11Texture2D* texture2D = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;
		ShaderResourceView* shaderResourceView = nullptr;

		int width = 1;
		int height = 1;

		QuadObject quad;

		void cleanUp();

	public:
		RenderToTexture();
		RenderToTexture(ID3D11Device* device, ID3D11DeviceContext* context);
		~RenderToTexture();
	
		HRESULT create(int width, int height);
		ShaderResourceView* getShaderResourceView();
		ID3D11RenderTargetView* getRenderTargetView();

		QuadObject& getQuad();
		void setShaders(std::string vs, std::string ps, std::string il);
		void draw();
		
		int getWidth();
		int getHeight();

};
}