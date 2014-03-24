#pragma once

#include "Timer.h"
#include "RenderContext.h"
#include "Camera.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace DDEngine
{
class D3DRenderer {

	private:

		void cleanUp();

	public:

		D3DRenderer();
		virtual ~D3DRenderer();

		void renderFrame();
		void setRenderWireframe(bool renderedWireframe);
		void setBackgroundColor( int r, int g, int b, int a );
		void setMultiSamplingValue(UINT multiSampling);

		ID3D11Device* getDevice();
		ID3D11DeviceContext* getContext();
		
		Timer& getTimer();
		Camera& getCamera();

	protected:

		bool renderStep = false;
		bool oneStep = false;

		Camera camera;
		Timer timer;
		RenderContext Ctx;
		Dimension screenDimension;

		virtual void onRender() = 0;
		HRESULT initialize(HWND hwnd);

};
}