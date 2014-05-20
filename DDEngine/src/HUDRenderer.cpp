#include "HUDRenderer.h"
#include "RenderContext.h"
#include "Config.h"
#include "Timer.h"
#include "DLLResourceLoader.h"
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

using namespace DDEngine;
using namespace std;
using namespace DirectX;

HUDRenderer::HUDRenderer(Config& config, RenderContext& renderContext, Timer& timer) : config(config), renderContext(renderContext), timer(timer) {
	resourcesHmodule = LoadLibrary(L"dderes.dll");
	isRendered = true;
	
	DataContainer fontData = DLLResourceLoader::loadHUD_spritefont(resourcesHmodule);

	spriteBatch.reset(new SpriteBatch(renderContext.context));
	spriteFont.reset(new SpriteFont(renderContext.device, (uint8_t*) fontData.dataBlob, (size_t) fontData.dataSize));
}

HUDRenderer::~HUDRenderer() {
	FreeLibrary(resourcesHmodule);
}

void HUDRenderer::setRender(string hudKey, bool isRender) {
	HUDBuffer::iterator it = buffer.find(hudKey);
	if(it != buffer.end()) {
		HUD* hud = &(it->second);
		hud->render = isRender;
	}
}

void HUDRenderer::render() {
	if(isRendered) {
		timeCycle += timer.delta;

		spriteBatch->Begin();

		for (HUDBuffer::const_iterator it = buffer.begin(); it != buffer.end(); ++it) {
			HUD hud = it->second;
			if(hud.render) {

				string s = hud.text;
				wstring ws;
				ws.assign(s.begin(), s.end());
				spriteFont->DrawString(spriteBatch.get(), ws.c_str(), XMFLOAT2((FLOAT) hud.coords.x, (FLOAT) hud.coords.y), DirectX::XMLoadFloat3(&hud.textColor));
			}
		}

		spriteBatch->End();

		if (timeCycle > notificationTime) {
			timeCycle = 0;
			clearNotifications();
		}
	}
}

void HUDRenderer::setHUDRendered( bool isRendered ) {
	this->isRendered = isRendered;
}

void HUDRenderer::update(string hudKey, string text) {
	HUDBuffer::iterator it = buffer.find(hudKey);
	if(it != buffer.end()) {
		HUD* hud = &(it->second);
		hud->text = text;
	}
}

void HUDRenderer::addText(string name, string text, float x, float y, XMVECTOR color /*= DirectX::Colors::White*/, bool renderedByDefault /*= false*/) {
	HUD hud;
	POINT coords;

	coords.x = (LONG) x;
	coords.y = (LONG) y;

	hud.coords = coords;
	hud.hudObject = NULL;
	hud.name = name;
	hud.render = renderedByDefault;
	hud.text = text;
	XMStoreFloat3(&hud.textColor, color);

	buffer.insert(HUDBuffer::value_type(hud.name, hud));
}

void HUDRenderer::removeText( string name ) {
	buffer.erase(name);
}

void HUDRenderer::notification(string text, long time, DirectX::XMVECTOR color /*= DirectX::Colors::White*/) {
	notificationTime = time;
	timeCycle = 0;
	clearNotifications();
	addText("hud_notification", text, 5, (float) config.CFG_SCREEN_HEIGHT - 60, color, true);
}

void DDEngine::HUDRenderer::clearNotifications() {
	removeText("hud_notification");
}
