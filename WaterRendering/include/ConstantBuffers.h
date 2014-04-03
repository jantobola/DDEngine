#pragma once

#include <windows.h>
#include <directxmath.h>

struct Matrices_CB 
{

	Matrices_CB() {
		ZeroMemory(this, sizeof(Matrices_CB));
	}

	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

struct Light_CB
{
	Light_CB() {
		ZeroMemory(this, sizeof(Light_CB));
	}

	DirectX::XMFLOAT3 direction;
	float pad;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4X4 identity;

};

struct WVP_CB
{
	WVP_CB() {
		ZeroMemory(this, sizeof(WVP_CB));
	}

	DirectX::XMFLOAT4X4 WVP;
};

struct Timer_CB
{
	Timer_CB() {
		ZeroMemory(this, sizeof(Timer_CB));
	}

	float time = 1.0f;
	DirectX::XMFLOAT3 pad;
};