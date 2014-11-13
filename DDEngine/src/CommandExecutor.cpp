#include "CommandExecutor.h"
#include "DDEComponent.h"
#include "ResourceProvider.h"
#include "HUDRenderer.h"
#include "Controlls.h"
#include "ScenesManager.h"
#include "ShaderHolder.h"
#include "DDEUtils.h"
#include "Object3D.h"
#include "SceneTransformator.h"

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

	EXECUTE("camera.reset") {
		component.getCamera().resetCamera();
		INFO("Camera position reset", 2500)
		END
	}

	EXECUTE("camera.ortho") {
		component.getCamera().toOrthographicProjection();
		INFO("Orthographic projection activated", 2500)
		END
	}

	EXECUTE("camera.persp") {
		component.getCamera().toPerspectiveProjection();
		INFO("Perspective projection activated", 2500)
		END
	}

	EXECUTE("camera.save") {
		component.getCamera().saveCamera();
		INFO("Camera position saved", 2500)
		END
	}

	EXECUTE("camera.load") {
		component.getCamera().loadCamera();
		INFO("Camera position loaded", 2500)
		END
	}

	EXECUTE("shaders.listenAll") {
		component.getResources().getShaderHolder().runRealTimeCompilerAll();
		component.getHUD().addText("shaders", "Shaders are listening to all changes.", 100.0f, 10.0f, DirectX::Colors::White, true);
		END
	}

	EXECUTE("shaders.stop") {
		component.getResources().getShaderHolder().stopRealTimeCompiler();
		component.getHUD().removeText("shaders");
		INFO("Shader compiler deactivated", 2500)
		END
	}

// ############################################################
	CMD_ARGS(1) // Commands with one argument #################
// ############################################################

	EXECUTE("transform.enable") {
		component.getScenesManager().getTransformator().setEnabledTransformations(ARG_BOOL(0));
		END
	}

	EXECUTE("transform.select") {
		component.getScenesManager().getTransformator().selectObject(component.getRenderContext().getRegisteredObjectIndex(ARG(0)));
		END
	}

	EXECUTE("hud.fps") {
		component.getHUD().setRender("fps", ARG_BOOL(0));
		END
	}

	EXECUTE("renderer.wireframe") {
		component.setRenderWireframe(ARG_BOOL(0));
		END
	}

	EXECUTE("input.sensitivity") {
		component.getControlls().setSensitivity(ARG_FLOAT(0));
		INFO("Mouse sensitivity changed", 2500)
		END
	}

	EXECUTE("input.speed") {
		component.getControlls().setSpeed(ARG_FLOAT(0));
		INFO("Move speed changed", 2500)
		END
	}

	EXECUTE("input.inverted") {
		component.getControlls().setMouseInverted(ARG_BOOL(0));
		if(ARG_BOOL(0)) INFO("Mouse movement is now inverted", 2500)
		else INFO("Mouse movement is now normal", 2500)
		END
	}

	EXECUTE("hud.enable") {
		component.getHUD().setHUDRendered(ARG_BOOL(0));
		END
	}

	EXECUTE("shaders.recompileVS") {
		component.getResources().getShaderHolder().recompileVertexShader(ARG(0));
		INFO("Recompiling vertex shader: " + ARG(0), 2500);
		END
	}

	EXECUTE("shaders.recompilePS") {
		component.getResources().getShaderHolder().recompilePixelShader(ARG(0));
		INFO("Recompiling pixel shader: " + ARG(0), 2500);
		END
	}

	EXECUTE("scenes.hide") {
		component.getScenesManager().hide(ARG(0));
		INFO(ARG(0) + " scene hidden", 2500);
		END
	}

	EXECUTE("scenes.show") {
		component.getScenesManager().show(ARG(0));
		INFO(ARG(0) + " scene shown", 2500);
		END
	}

	EXECUTE("objects.hide") {
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->setVisible(false);
		INFO(ARG(0) + " object hidden", 2500);
		END
	}

	EXECUTE("objects.show") {
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->setVisible(true);
		INFO(ARG(0) + " object shown", 2500);
		END
	}

	EXECUTE("renderer.normals") {
		for (AbstractObject* o : component.getRenderPackage().renderContext.getRegisteredObjects()) {
			o->setShowNormals(ARG_BOOL(0));
		}
		if (ARG_BOOL(0)) INFO("All normals are visualized", 2500);
		END
	}

// ############################################################
	CMD_ARGS(2) // Commands with two arguments ################
// ############################################################

	EXECUTE("camera.znear_zfar") {
		component.getCamera().changeNearFar(ARG_FLOAT(0), ARG_FLOAT(1));
		INFO("Camera ZNEAR, ZFAR changed", 2500);
		END
	}

	EXECUTE("hud.draw") {
		component.getHUD().setRender(ARG(0), ARG_BOOL(1));
		END
	}

	EXECUTE("objects.rotateX") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotateX(ARG_FLOAT(1));
		INFO("Object rotated", 2500);
		END
	}

	EXECUTE("objects.rotateY") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotateY(ARG_FLOAT(1));
		INFO("Object rotated", 2500);
		END
	}

	EXECUTE("objects.rotateZ") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotateZ(ARG_FLOAT(1));
		INFO("Object rotated", 2500);
		END
	}

	EXECUTE("objects.translateX") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(ARG_FLOAT(1), 0, 0);
		INFO("Object translated", 2500);
		END
	}

	EXECUTE("objects.translateY") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(0, ARG_FLOAT(1), 0);
		INFO("Object translated", 2500);
		END
	}

	EXECUTE("objects.translateZ") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(0, 0, ARG_FLOAT(1));
		INFO("Object translated", 2500);
		END
	}

	EXECUTE("objects.scale") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(ARG_FLOAT(1));
		INFO("Object scaled", 2500);
		END
	}

	EXECUTE("objects.scaleX") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(ARG_FLOAT(1), 1, 1);
		INFO("Object scaled", 2500);
		END
	}

	EXECUTE("objects.scaleY") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(1, ARG_FLOAT(1), 1);
		INFO("Object scaled", 2500);
		END
	}

	EXECUTE("objects.scaleZ") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(1, 1, ARG_FLOAT(1));
		INFO("Object scaled", 2500);
		END
	}

	EXECUTE("objects.normals") {
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->setShowNormals(ARG_BOOL(1));
		if(ARG_BOOL(1)) INFO(ARG(0) + " normals are visualized", 2500);
		END
	}

// ############################################################
	CMD_ARGS(3) // Commands with three arguments ##############
// ############################################################

	EXECUTE("renderer.bgcolor") {
		component.setBackgroundColor(ARG_INT(0), ARG_INT(1), ARG_INT(2), 255);
		INFO("Background window color changed", 2500);
		END
	}

// ############################################################
	CMD_ARGS(4) // Commands with four arguments ###############
// ############################################################

	EXECUTE("objects.rotate") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->rotate(ARG_FLOAT(1), ARG_FLOAT(2), ARG_FLOAT(3));
		INFO("Object rotated", 2500);
		END
	}

	EXECUTE("objects.translate") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->translate(ARG_FLOAT(1), ARG_FLOAT(2), ARG_FLOAT(3));
		INFO("Object translated", 2500);
		END
	}

	EXECUTE("objects.scale") {
		if (component.getRenderPackage().renderContext.getRegisteredObject(ARG(0)))
		component.getRenderPackage().renderContext.getRegisteredObject(ARG(0))->scale(ARG_FLOAT(1), ARG_FLOAT(2), ARG_FLOAT(3));
		INFO("Object scaled", 2500);
		END
	}

// ############################################################
	QUIT // Quit statement ####################################
// ############################################################
}