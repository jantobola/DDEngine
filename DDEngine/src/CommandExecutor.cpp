#include "CommandExecutor.h"
#include "DDEComponent.h"
#include "ResourceProvider.h"
#include "HUDRenderer.h"
#include "Controlls.h"
#include "ObjectManager.h"
#include "ShaderHolder.h"
#include "DDEUtils.h"

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

void CommandExecutor::executeCommand(std::string command) {
	START(command)
	
// ############################################################
	CMD_NO_ARGS // Commands without arguments #################
// ############################################################

	EXECUTE("quit") {
		component.quit();
		END
	}

	EXECUTE("cam_reset") {
		component.getCamera().resetCamera();
		END
	}

	EXECUTE("cam_ortho") {
		component.getCamera().toOrthographicProjection();
		END
	}

	EXECUTE("cam_persp") {
		component.getCamera().toPerspectiveProjection();
		END
	}

	EXECUTE("shaders_listen_all") {
		component.getResources()->getShaderHolder().runRealTimeCompilerAll();
		component.getHUD()->addText("shaders", "Shaders are listening to all changes.", 100.0f, 10.0f, DirectX::Colors::Red, true);
		END
	}

	EXECUTE("shaders_stop") {
		component.getResources()->getShaderHolder().stopRealTimeCompiler();
		component.getHUD()->removeText("shaders");
		END
	}

// ############################################################
	CMD_ARGS(1) // Commands with one argument #################
// ############################################################

	EXECUTE("show_fps") {
		component.getHUD()->setRender("fps", ARG_BOOL(0));
		END
	}

	EXECUTE("render_wireframe") {
		component.setRenderWireframe(ARG_BOOL(0));
		END
	}

	EXECUTE("mouse_sensitivity") {
		component.getControlls()->setSensitivity(ARG_FLOAT(0));
		END
	}

	EXECUTE("move_speed") {
		component.getControlls()->setSpeed(ARG_FLOAT(0));
		END
	}

	EXECUTE("mouse_inverted") {
		component.getControlls()->setMouseInverted(ARG_BOOL(0));
		END
	}

	EXECUTE("hud_enable") {
		component.getHUD()->setHUDRendered(ARG_BOOL(0));
		END
	}

	EXECUTE("shaders_recompileVS") {
		component.getResources()->getShaderHolder().recompileVertexShader(ARG(0));
		END
	}

	EXECUTE("shaders_recompilePS") {
		component.getResources()->getShaderHolder().recompilePixelShader(ARG(0));
		END
	}

	EXECUTE("object_hide") {
		component.getObjectManager()->hide(ARG(0));
		END
	}

	EXECUTE("object_show") {
		component.getObjectManager()->show(ARG(0));
		END
	}

// ############################################################
	CMD_ARGS(2) // Commands with two arguments ################
// ############################################################

	EXECUTE("cam_znear_zfar") {
		component.getCamera().changeNearFar(ARG_FLOAT(0), ARG_FLOAT(1));
		END
	}

	EXECUTE("hud_draw") {
		component.getHUD()->setRender(ARG(0), ARG_BOOL(1));
		END
	}

	EXECUTE("shaders_listen_on") {
		component.getResources()->getShaderHolder().runRealTimeCompiler(ARG(0), ARG(1));
		END
	}

// ############################################################
	CMD_ARGS(3) // Commands with three arguments ##############
// ############################################################

// ############################################################
	CMD_ARGS(4) // Commands with four arguments ###############
// ############################################################

	EXECUTE("render_bgcolor") {
		component.setBackgroundColor(ARG_INT(0), ARG_INT(1), ARG_INT(2), ARG_INT(3));
		END
	}

// ############################################################
	QUIT // Quit statement ####################################
// ############################################################
}

void DDEngine::CommandExecutor::executeCommand(std::string command, std::function<void()>&& funct) {
	funct();
}
