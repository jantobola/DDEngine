#include "Controlls.h"
#include "Camera.h"
#include "Timer.h"
#include <string.h>

using namespace DDEngine;

Controlls::Controlls(Camera& camera, Timer& timer) : camera(camera), timer(timer) {
	mouseSensitivity = 3.0f;
	moveSpeed = 3.0f;
	mouseInverted = true;

	KEY_MOVE_FORWARD	=	'W';
	KEY_MOVE_BACKWARD	=	'S';
	KEY_STRAFE_LEFT		=	'A';
	KEY_STRAFE_RIGHT	=	'D';
}

Controlls::~Controlls() {

}

void Controlls::moveForward() {
	camera.moveForward(timer.velocity(), moveSpeed);
}

void Controlls::moveBackward() {
	camera.moveBackward(timer.velocity(), moveSpeed);
}

void Controlls::strafeRight() {
	camera.strafeRight(timer.velocity(), moveSpeed);
}

void Controlls::strafeLeft() {
	camera.strafeLeft(timer.velocity(), moveSpeed);
}

void Controlls::mouseLook( LONG deltaX, LONG deltaY ) {
	camera.camYaw += deltaX * 0.001f * mouseSensitivity;
	if(mouseInverted) camera.camPitch -= deltaY * 0.001f * mouseSensitivity;
	else camera.camPitch += deltaY * 0.001f * mouseSensitivity;
}

void Controlls::setSensitivity( float sensitivity ) {
	this->mouseSensitivity = sensitivity;
}

void Controlls::setSpeed( float speed ) {
	this->moveSpeed = speed;
}

void Controlls::setMouseInverted( bool reversed ) {
	this->mouseInverted = reversed;
}
