#pragma once

#include "Dimension.h"
#include "Camera.h"
#include <d3d11.h>
#include <DirectXTK/CommonStates.h>
#include <vector>
#include <string>

namespace DDEngine
{

class AbstractObject;
class ShaderHolder;

typedef ID3D11ShaderResourceView ShaderResourceView;
typedef ID3D11SamplerState SamplerState;

enum RasterizerStateType {
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIRED_CULL_NONE,
};

enum PrimitiveTopology {
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST,
	LINE_STRIP,
	POINT_LIST,
	UNDEFINED
};

D3D11_PRIMITIVE_TOPOLOGY translatePrimitiveTopology(PrimitiveTopology topology);

class RenderContext {

	private:
		std::vector<AbstractObject*> objectContainer;
		ShaderHolder* shaderHolder;
		
	public:
		RenderContext();
		~RenderContext();

		RasterizerStateType currentRasterizerState;
		UINT multiSampling;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGISwapChain*	swapChain;
		ID3D11RasterizerState* RSWiredCullNone;
		ID3D11RasterizerState* RSWiredCullBack;
		ID3D11RasterizerState* RSSolidCullBack;
		ID3D11RasterizerState* RSSolidCullNone;
		ID3D11BlendState* blendState;
		ID3D11RenderTargetView*	renderTargetView;
		ID3D11DepthStencilView*	depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;

		DirectX::CommonStates* commonStates;
		Camera* camera;

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
		void clearRenderTarget(float bgColor[], ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStencilView);
		void setViewport(int x, int y, int width, int height);
		
		void setPSResource(ShaderResourceView* resource, int index);
		void setVSResource(ShaderResourceView* resource, int index);
		void setPSResourceArray(std::vector<ShaderResourceView*>& resources, int index);
		void setVSResourceArray(std::vector<ShaderResourceView*>& resources, int index);

		void setPSSampler(SamplerState* sampler, int index);
		void setVSSampler(SamplerState* sampler, int index);

		inline ShaderHolder* getShaderHolder() { return shaderHolder; }
		inline void setShaderHolder(ShaderHolder* shaderHolder) { this->shaderHolder = shaderHolder; }

		inline std::vector<AbstractObject*>& getRegisteredObjects() { return objectContainer; }
		AbstractObject* getRegisteredObject(const std::string& name);
		const int getRegisteredObjectIndex(const std::string& name);

	protected:
	
};
}