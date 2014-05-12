#pragma once

#include <windows.h>

namespace DDEngine
{

class Camera;
class Timer;

class Controls {
	
	public:
		int KEY_MOVE_FORWARD;
		int KEY_MOVE_BACKWARD;
		int KEY_STRAFE_LEFT;
		int KEY_STRAFE_RIGHT;

	private:
		Camera& camera;
		Timer& timer;

		float moveSpeed;
		float mouseSensitivity;
		bool mouseInverted;

	public:
		Controls(Camera& camera, Timer& timer);
		~Controls();

		void moveForward();
		void moveBackward();
		void strafeRight();
		void strafeLeft();

		void mouseLook(LONG deltaX, LONG deltaY);

		void setSensitivity(float sensitivity);
		void setSpeed(float speed);
		void setMouseInverted(bool reversed);
};
}