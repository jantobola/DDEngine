#pragma once

#include <unordered_map>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <memory>
#include <string>
#include <windows.h>

namespace DirectX
{
class SpriteFont;
class SpriteBatch;
}

namespace DDEngine
{

class RenderContext;
class Config;

struct HUD
{
	std::string name;
	std::string text;
	DirectX::XMFLOAT3 textColor;
	POINT coords;
	bool render;

	void* hudObject;
};

class HUDRenderer {

	typedef std::string string;

	private:
		
		typedef std::unordered_map<std::string, HUD> HUDBuffer;
		
		HUDBuffer buffer;
		std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
		std::unique_ptr<DirectX::SpriteFont> spriteFont;

		Config& config;
		RenderContext& renderContext;
		bool isRendered;

		HMODULE resourcesHmodule;

	public:
		HUDRenderer(Config& config, RenderContext& renderContext);
		~HUDRenderer();

		void addText(string name, string text, float x, float y, DirectX::XMVECTOR color = DirectX::Colors::White, bool renderedByDefault = false);
		void update(string hudKey, string text);
		void setRender(string hudKey, bool isRender);
		void render();
		void setHUDRendered(bool isRendered);
		void removeText(string name);
};
}