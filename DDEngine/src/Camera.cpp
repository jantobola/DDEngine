#include "Camera.h"
#include "DDEUtils.h"
#include <iostream>
#include <fstream>

using namespace DDEngine;
using namespace DirectX;
using namespace std;

Camera::Camera() {
	isPerspectiveProjection = true;
	zNear = 0.01f;
	zFar = 100.0f;

	resetCamera();

	XMMATRIX w = XMMatrixIdentity();
	XMMATRIX v = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up));
	
	XMStoreFloat4x4(&world, w);
	XMStoreFloat4x4(&view, v);
}

Camera::~Camera() {

}

void Camera::resetCamera() {

	//Set default camera information
	XMVECTOR e = XMVectorSet( 0.0f, 0.0f, -1.0f, 0.0f );
	XMVECTOR a = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	XMVECTOR u = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	XMStoreFloat3(&eye, e);
	XMStoreFloat3(&at, a);
	XMStoreFloat3(&up, u);

	setDefaultState();
}

void Camera::update() {

	XMVECTOR e = XMLoadFloat3(&eye);
	XMVECTOR a = XMLoadFloat3(&at);
	XMVECTOR u = XMLoadFloat3(&up);

	float ninety = XMConvertToRadians(90);
	float nninety = XMConvertToRadians(-90);

	if (camPitch >= ninety) {
		camPitch = ninety;
	} else if (camPitch <= nninety) {
		camPitch = nninety;
	}

	XMStoreFloat4x4(&camRotationMatrix, XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0));
	XMMATRIX camRotMatrix = XMLoadFloat4x4(&camRotationMatrix);

	a = XMVector3TransformCoord(XMLoadFloat3(&defaultForward), camRotMatrix);
	a = XMVector3Normalize(a);

	// Free-Look Camera
	XMStoreFloat3(&camRight, XMVector3TransformCoord(XMLoadFloat3(&defaultRight), camRotMatrix));
	XMStoreFloat3(&camForward, XMVector3TransformCoord(XMLoadFloat3(&defaultForward), camRotMatrix));
	u = XMVector3Cross(XMLoadFloat3(&camForward), XMLoadFloat3(&camRight));

	e += moveLeftRight * XMLoadFloat3(&camRight);
	e += moveBackForward * XMLoadFloat3(&camForward);

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	a = e + a;

	XMStoreFloat3(&eye, e);
	XMStoreFloat3(&at, a);
	XMStoreFloat3(&up, u);

	XMMATRIX v = XMMatrixLookAtLH(e, a, u);
	XMStoreFloat4x4(&view, v);
}

void DDEngine::Camera::makeReflection(float height) {
	XMFLOAT3 position, target, side;
	XMVECTOR forwardVector, sideVector, positionVector, targetVector, upVector;
	XMMATRIX reflectionViewMatrix;

	position.x = eye.x;
	position.y = -eye.y + (2.0f * height);
	position.z = eye.z;

	target.x = at.x;
	target.y = -at.y + (2.0f * height);
	target.z = at.z;

	positionVector = XMLoadFloat3(&position);
	targetVector = XMLoadFloat3(&target);

	side.x = 1;
	side.y = 0;
	side.z = 0;

	XMVECTOR sideVec = XMLoadFloat3(&side);

	forwardVector = XMVectorSubtract(targetVector, positionVector);
	sideVector = XMVector3TransformCoord(sideVec, XMLoadFloat4x4(&camRotationMatrix));
	upVector = XMVector3Cross(sideVector, forwardVector);

	reflectionViewMatrix = XMMatrixLookAtLH(positionVector, targetVector, upVector);
	XMStoreFloat4x4(&reflectionView, reflectionViewMatrix);
}

void DDEngine::Camera::setViewMatrix(XMFLOAT4X4 viewMatrix) {
	view = viewMatrix;
}

void DDEngine::Camera::saveCamera() {

	ofstream saveFile(savePath);
	if (saveFile.is_open()) {

		saveFile << to_string(eye.x) << endl;
		saveFile << to_string(eye.y) << endl;
		saveFile << to_string(eye.z) << endl;
		saveFile << to_string(camYaw) << endl;
		saveFile << to_string(camPitch);
		
		saveFile.close();
	}
}

void DDEngine::Camera::loadCamera() {
	string line;
	int iter = 0;
	ifstream saveFile(savePath);
	if (saveFile.is_open()) {
		while (getline(saveFile, line)) {
			iter++;
			float value = StringUtils::toFloat(line);

			switch (iter) {

				case 1:
					eye.x = value;
					break;

				case 2:
					eye.y = value;
					break;

				case 3:
					eye.z = value;
					break;

				case 4:
					camYaw = value;
					break;

				case 5:
					camPitch = value;
					break;

				default:
					break;
			}
		}

		saveFile.close();
	}
}

void DDEngine::Camera::setSavePath(std::string path) {
	if (!path.empty()) savePath = path + "/save.cam";
	else savePath = "save.cam";
}

void Camera::setDefaultState() {

	XMStoreFloat3(&defaultForward, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	XMStoreFloat3(&defaultRight, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	XMStoreFloat3(&camForward, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	XMStoreFloat3(&camRight, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camYaw = 0.0f;
	camPitch = 0.0f;
}

void Camera::setProjectionMatrices(Dimension screenDimension) {
	this->screenDimension = screenDimension;

	XMMATRIX perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT) screenDimension.WIDTH / (FLOAT) screenDimension.HEIGHT, zNear, zFar);
	XMMATRIX orthographic = XMMatrixOrthographicLH((FLOAT) screenDimension.WIDTH, (FLOAT) screenDimension.HEIGHT, zNear, zFar);

	XMStoreFloat4x4(&perspectiveProjection, perspective);
	XMStoreFloat4x4(&orthographicProjection, orthographic);

	if(isPerspectiveProjection) projection = perspectiveProjection;
	else projection = orthographicProjection;
}

void Camera::toPerspectiveProjection() {
	projection = perspectiveProjection;
	isPerspectiveProjection = true;
}

void Camera::toOrthographicProjection() {
	projection = orthographicProjection;
	isPerspectiveProjection = false;
}

void Camera::changeNearFar( float zNear, float zFar ) {
	this->zNear = zNear;
	this->zFar = zFar;

	setProjectionMatrices(screenDimension);
}

XMMATRIX Camera::getViewMatrix() {
	return XMLoadFloat4x4(&view);
}

XMMATRIX Camera::getReflectionViewMatrix() {
	return XMLoadFloat4x4(&reflectionView);
}

XMMATRIX Camera::getProjectionMatrix() {
	return XMLoadFloat4x4(&projection);
}

XMMATRIX Camera::getPerspectiveMatrix() {
	return XMLoadFloat4x4(&perspectiveProjection);
}

XMMATRIX Camera::getOrthographicMatrix() {
	return XMLoadFloat4x4(&orthographicProjection);
}

void Camera::moveForward( float velocity, float speed ) {
	moveBackForward += speed * velocity;
}

void Camera::moveBackward( float velocity, float speed ) {
	moveBackForward -= speed * velocity;
}

void Camera::strafeRight( float velocity, float speed ) {
	moveLeftRight += speed * velocity;
}

void Camera::strafeLeft( float velocity, float speed ) {
	moveLeftRight -= speed * velocity;
}