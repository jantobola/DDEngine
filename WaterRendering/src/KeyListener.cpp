#include "KeyListener.h"
#include "CustomRenderer.h"
#include "RenderableWater.h"
#include <AntTweakBar/AntTweakBar.h>

using namespace DDEngine;

void KeyListener::onKeyTyped(WPARAM wParam)
{
	CommandExecutor& cmd = console->getExecutor();

	//ESCAPE - toggle AntTweakBar
	KEY(VK_ESCAPE, wParam) {
		if (tweakBarHidden) {
			TwDefine(" lightBar visible=true ");
			TwDefine(" waterBar visible=true ");
			TwDefine(" terrainBar visible=true ");
			tweakBarHidden = false;
		}
		else {
			TwDefine(" lightBar visible=false ");
			TwDefine(" waterBar visible=false ");
			TwDefine(" terrainBar visible=false ");
			tweakBarHidden = true;
		}
	}

	// ENTER - water drop
	KEY(VK_RETURN, wParam) {
		renderer->water->setWaterDrop(true);
	}

	KEY(VK_SPACE, wParam) {
		renderer->water->setWaterDrop(true);
	}

	KEY(VK_BACK, wParam) {
		renderer->water->resetSurface(true);
	}

	// start stepping water
	KEY(VK_NUMPAD1, wParam) {
		renderer->water->setStepping(true);
	}

	KEY(VK_NUMPAD2, wParam) {
		renderer->water->setStepping(false);
	}

	KEY(VK_NUMPAD0, wParam) {
		renderer->water->step();
	}

}

void KeyListener::onKeyDownAsync()
{

}

KeyListener::KeyListener(CustomRenderer* renderer) {
	this->renderer = renderer;
}
