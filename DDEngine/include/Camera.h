#pragma once

#include "Dimension.h"
#include <DirectXMath.h>

namespace DDEngine
{
class Camera {

	typedef DirectX::XMFLOAT4X4 XMFLOAT4X4;
	typedef DirectX::XMFLOAT3 XMFLOAT3;
	typedef DirectX::XMMATRIX XMMATRIX;

	private:

		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;

		XMFLOAT4X4 perspectiveProjection;
		XMFLOAT4X4 orthographicProjection;

		XMFLOAT3 defaultForward;
		XMFLOAT3 defaultRight;
		XMFLOAT3 camForward;
		XMFLOAT3 camRight;

		XMFLOAT4X4 camRotationMatrix;

		float moveLeftRight;
		float moveBackForward;

		bool isPerspectiveProjection;
		float zNear;
		float zFar;

		Dimension screenDimension;

		void setDefaultState();

	public:

		XMFLOAT3 eye;
		XMFLOAT3 at;
		XMFLOAT3 up;

		float camYaw;
		float camPitch;

		Camera();
		~Camera();

		void moveForward(float velocity, float speed);
		void moveBackward(float velocity, float speed);
		void strafeRight(float velocity, float speed);
		void strafeLeft(float velocity, float speed);

		void setProjectionMatrices(Dimension screenDimension);
		void toPerspectiveProjection();
		void toOrthographicProjection();
		void changeNearFar(float zNear, float zFar);

		XMMATRIX getViewMatrix();
		XMMATRIX getProjectionMatrix();
		XMMATRIX getPerspectiveMatrix();
		XMMATRIX getOrthographicMatrix();

		void resetCamera();
		void update();
};
}