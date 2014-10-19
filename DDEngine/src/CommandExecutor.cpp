#include "CommandExecutor.h"
#include "DDEComponent.h"
#include "ResourceProvider.h"
#include "HUDRenderer.h"
#include "Controlls.h"
#include "ScenesManager.h"
#include "ShaderHolder.h"
#include "DDEUtils.h"
#include "Object3D.h"

using namespace DDEngine;

#define START(CMD) std::vector<std::string> tokens; \
	parseCommand(CMD, tokens); \
	manageHistory(CMD);
#define END executed = true; return;
#define QUIT executed = false;
#define ARG(X) tokens.at(X + 1) 
#define ARG_BOOL(X) StringUtils::toBool(ARG(X))
#define ARG_FLOAT(X) StringUtils::toFloat(ARG(X))
#define ARG_INT(X) StringUtils::toInt(ARG(X))
#define EXECUTE(CMD) if(tokens.at(0) == CMD)
#define CMD_NO_ARGS if(tokens.size() < 1) return; 
#define CMD_ARGS(X) if(tokens.size() < (X + 1)) return;
#define INFO(X, T) component.getHUD().notification(X, T);
#define DELEGATE(X) if(_delegate) { _delegate(X); }

void DDEngine::CommandExecutor::delegate(std::function<void(const std::vector<std::string>&)> func) {
	this->_delegate = func;
}

void CommandExecutor::executeCommand(std::string command) {
	START(command)
	DELEGATE(tokens)

// ############################################################
	CMD_NO_ARGS // Commands without arguments #################
// ############################################################

	EXECUTE("quit") {
		component.quit();
		END
	}

	EXECUTE("cam_reset") {
		component.getCamera().resetCamera();
		INFO("Camera position reset", 2500)
		END
	}

	EXECUTE("cam_ortho") {
		component.getCamera().toOrthographicProjection();
		INFO("Orthographic projection activated", 2500)
		END
	}

	EXECUTE("cam_persp") {
		component.getCamera().toPerspectiveProjection();
		INFO("Perspective projection activated", 2500)
		END
	}

	EXECUTE("cam_save") {
		component.getCamera().saveCamera();
		INFO("Camera position saved", 2500)
		END
	}

	EXECUTE("cam_load") {
		component.getCamera().loadCamera();
		INFO("Camera position loaded", 2500)
		END
	}

	EXECUTE("shaders_listen_all") {
		component.getResources().getShaderHolder().runRealTimeCompilerAll();
		component.getHUD().addText("shaders", "Shaders are listening to all changes.", 100.0f, 10.0f, DirectX::Colors::White, true);
		END
	}

	EXECUTE("shaders_stop") {
		component.getResources().getShaderHolder().stopRealTimeCompiler();
		component.getHUD().removeText("shaders");
		INFO("Shader compiler deactivated", 2500)
		END
	}

// ############################################################
	CMD_ARGS(1) // Commands with one argument #################
// ############################################################

	EXECUTE("show_fps") {
		component.getHUD().setRender("fps", ARG_BOOL(0));
		END
	}

	EXECUTE("render_wireframe") {
		component.setRenderWireframe(ARG_BOOL(0));
		END
	}

	EXECUTE("mouse_sensitivity") {
		component.getControlls().setSensitivity(ARG_FLOAT(0));
		INFO("Mouse sensitivity changed", 2500)
		END
	}

	EXECUTE("move_speed") {
		component.getControlls().setSpeed(ARG_FLOAT(0));
		INFO("Move speed changed", 2500)
		END
	}

	EXECUTE("mouse_inverted") {
		component.getControlls().setMouseInverted(ARG_BOOL(0));
		if(ARG_BOOL(0)) INFO("Mouse movement is now inverted", 2500)
		else INFO("Mouse movement is now normal", 2500)
		END
	}

	EXECUTE("hud_enable") {
		component.getHUD().setHUDRendered(ARG_BOOL(0));
		END
	}

	EXECUTE("shaders_recompileVS") {
		component.getResources().getShaderHolder().recompileVertexShader(ARG(0));
		INFO("Recompiling vertex shader: " + ARG(0), 2500);
		END
	}

	EXECUTE("shaders_recompilePS") {
		component.getResources().getShaderHolder().recompilePixelShader(ARG(0));
		INFO("Recompiling pixel shader: " + ARG(0), 2500);
		END
	}

	EXECUTE("scene_hide") {
		component.getScenesManager().hide(ARG(0));
		INFO(ARG(0) + " scene hidden", 2500);
		END
	}

	EXECUTE("scene_show") {
		component.getScenesManager().show(ARG(0));
		INFO(ARG(0) + " scene shown", 2500);
		END
	}

	EXECUTE("object_hide") {
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->setVisible(false);
		INFO(ARG(0) + " object hidden", 2500);
		END
	}

	EXECUTE("object_show") {
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->setVisible(true);
		INFO(ARG(0) + " object shown", 2500);
		END
	}

// ############################################################
	CMD_ARGS(2) // Commands with two arguments ################
// ############################################################

	EXECUTE("cam_znear_zfar") {
		component.getCamera().changeNearFar(ARG_FLOAT(0), ARG_FLOAT(1));
		INFO("Camera ZNEAR, ZFAR changed", 2500);
		END
	}

	EXECUTE("hud_draw") {
		component.getHUD().setRender(ARG(0), ARG_BOOL(1));
		END
	}

	EXECUTE("rotateX") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotateX(ARG_FLOAT(1));
		INFO("Object rotated...", 2500);
		END
	}

	EXECUTE("rotateY") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotateY(ARG_FLOAT(1));
		INFO("Object rotated...", 2500);
		END
	}

	EXECUTE("rotateZ") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotateZ(ARG_FLOAT(1));
		INFO("Object rotated...", 2500);
		END
	}

	EXECUTE("translateX") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(ARG_FLOAT(1), 0, 0);
		INFO("Object translated...", 2500);
		END
	}

	EXECUTE("translateY") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(0, ARG_FLOAT(1), 0);
		INFO("Object translated...", 2500);
		END
	}

	EXECUTE("translateZ") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(0, 0, ARG_FLOAT(1));
		INFO("Object translated...", 2500);
		END
	}

	EXECUTE("scale") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(ARG_FLOAT(1));
		INFO("Object scaled...", 2500);
		END
	}

	EXECUTE("scaleX") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(ARG_FLOAT(1), 0, 0);
		INFO("Object scaled...", 2500);
		END
	}

	EXECUTE("scaleY") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(0, ARG_FLOAT(1), 0);
		INFO("Object scaled...", 2500);
		END
	}

	EXECUTE("scaleZ") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(0, 0, ARG_FLOAT(1));
		INFO("Object scaled...", 2500);
		END
	}

// ############################################################
	CMD_ARGS(3) // Commands with three arguments ##############
// ############################################################

	EXECUTE("render_bgcolor") {
		component.setBackgroundColor(ARG_INT(0), ARG_INT(1), ARG_INT(2), 255);
		INFO("Background window color changed", 2500);
		END
	}

// ############################################################
	CMD_ARGS(4) // Commands with four arguments ###############
// ############################################################

	EXECUTE("rotate") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotate(ARG_FLOAT(1), ARG_FLOAT(2), ARG_FLOAT(3));
		INFO("Object rotated...", 2500);
		END
	}

	EXECUTE("translate") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(ARG_FLOAT(1), ARG_FLOAT(2), ARG_FLOAT(3));
		INFO("Object translated...", 2500);
		END
	}

	EXECUTE("scale") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(ARG_FLOAT(1), ARG_FLOAT(2), ARG_FLOAT(3));
		INFO("Object scaled...", 2500);
		END
	}

// ############################################################
	QUIT // Quit statement ####################################
// ############################################################
}