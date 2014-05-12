#include "Controlls.h"
#include "Camera.h"
#include "Timer.h"
#include <string.h>

using namespace DDEngine;

Controls::Controls(Camera& camera, Timer& timer) : camera(camera), timer(timer) {
	mouseSensitivity = 3.0f;
	moveSpeed = 3.0f;
	mouseInverted = true;

	KEY_MOVE_FORWARD	=	'W';
	KEY_MOVE_BACKWARD	=	'S';
	KEY_STRAFE_LEFT		=	'A';
	KEY_STRAFE_RIGHT	=	'D';
}

Controls::~Controls() {

}

void Controls::moveForward() {
	camera.moveForward(timer.velocity(), moveSpeed);
}

void Controls::moveBackward() {
	camera.moveBackward(timer.velocity(), moveSpeed);
}

void Controls::strafeRight() {
	camera.strafeRight(timer.velocity(), moveSpeed);
}

void Controls::strafeLeft() {
	camera.strafeLeft(timer.velocity(), moveSpeed);
}

void Controls::mouseLook( LONG deltaX, LONG deltaY ) {
	camera.camYaw += deltaX * 0.001f * mouseSensitivity;
	if(mouseInverted) camera.camPitch -= deltaY * 0.001f * mouseSensitivity;
	else camera.camPitch += deltaY * 0.001f * mouseSensitivity;
}

void Controls::setSensitivity( float sensitivity ) {
	this->mouseSensitivity = sensitivity;
}

void Controls::setSpeed( float speed ) {
	this->moveSpeed = speed;
}

void Controls::setMouseInverted( bool reversed ) {
	this->mouseInverted = reversed;
}
