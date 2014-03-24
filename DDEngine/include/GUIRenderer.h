#pragma once

#include "DDE_CEGUIResourceProvider.h"

namespace CEGUI
{
class Direct3D11Renderer;
}

namespace DDEngine
{

class RenderContext;
class Config;

class GUIRenderer {

	private:
		CEGUI::Direct3D11Renderer* guiRenderer;
		DDE_CEGUIResourceProvider ceguiResourceProvider;

		Config& config;
		RenderContext& renderContext;
		HMODULE resourcesHmodule;

		void initialize();
		void cleanUp();

	public:
		GUIRenderer(Config& config, RenderContext& renderContext);
		~GUIRenderer();

		void render();

};
}