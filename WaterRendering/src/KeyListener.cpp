#include "KeyListener.h"
#include "CustomRenderer.h"
#include "RenderableWater.h"
#include <AntTweakBar/AntTweakBar.h>

using namespace DDEngine;

void KeyListener::onKeyTyped(WPARAM wParam)
{
	CommandExecutor& cmd = console->getExecutor();

	// F1 - solid
	KEY(VK_F1, wParam) {
		cmd.executeCommand("render_wireframe = false");
	}

	//F2 - wireframe
	KEY(VK_F2, wParam) {
		cmd.executeCommand("render_wireframe = true");
	}

	//F5 - listen all shaders to changes
	KEY(VK_F5, wParam) {
		cmd.executeCommand("shaders_listen_all");
	}

	//F6 - stop listening to changes
	KEY(VK_F6, wParam) {
		cmd.executeCommand("shaders_stop");
	}

	//F8 - reset camera
	KEY(VK_F8, wParam) {
		cmd.executeCommand("cam_reset");
	}

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
