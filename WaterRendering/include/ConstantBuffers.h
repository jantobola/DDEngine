#pragma once

#include <windows.h>
#include <directxmath.h>

struct Matrices_CB 
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

struct Light_CB
{
	DirectX::XMFLOAT3 direction;
	float pad;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
};

struct WVP_CB
{
	DirectX::XMFLOAT4X4 WVP;
};

struct Timer_CB
{
	double time = 1.0;
	double timeCycle = 0.0;
	float timeStep = 1.0f;
	DirectX::XMFLOAT3 pad;
};

struct Camera_CB
{
	DirectX::XMFLOAT3 cameraPosition;
	float pad;
};