#pragma once

#include "Dimension.h"
#include <d3d11.h>

namespace DDEngine
{

enum RasterizerStateType {
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIRED_CULL_NONE,
};

class RenderContext {

	private:
		
	public:
		RenderContext();
		~RenderContext();

		RasterizerStateType currentRasterizerState;
		UINT multiSampling;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGISwapChain*	swapChain;
		ID3D11RasterizerState* RSWiredCullNone;
		ID3D11RasterizerState* RSSolidCullBack;
		ID3D11RasterizerState* RSSolidCullNone;
		ID3D11BlendState* blendState;
		ID3D11RenderTargetView*	renderTargetView;
		ID3D11DepthStencilView*	depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;

		Dimension screenDimension;
		float bgColor[4];
		bool wireframe;

		void setBackgroundColor( int r, int g, int b, int a );
		void setBackbufferRenderTarget();
		void setRenderWireframe(bool renderedWireframe);
		void setBlendState(ID3D11BlendState* blendState);
		void setRasterizerState(RasterizerStateType type);
		void setDefaultDepthStencilState();
		void setRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStencilView);
		void clearRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStencilView);
		void setViewport(int x, int y, int width, int height);
		void setPSResource(ID3D11ShaderResourceView* resource, int index);
		void setVSResource(ID3D11ShaderResourceView* resource, int index);
		void setPSSampler(ID3D11SamplerState* sampler, int index);
		void setVSSampler(ID3D11SamplerState* sampler, int index);

	protected:
	
};
}