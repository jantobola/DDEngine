#pragma once

#include <windows.h>
#include <directxmath.h>

namespace DDEngine
{

	namespace CB
	{

		struct WorldViewProjection
		{
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
		};

		struct GSMatrices : public CB::WorldViewProjection
		{
			DirectX::XMFLOAT4X4 invWorld;
			DirectX::XMFLOAT4X4 invView;
			DirectX::XMFLOAT4X4 invProjection;
			DirectX::XMFLOAT4X4 scale;
		};

		struct Light
		{
			DirectX::XMFLOAT3 lightDirection = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			float pad0;
			DirectX::XMFLOAT3 eyePosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			float pad1;
			DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
			DirectX::XMFLOAT4 diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			DirectX::XMFLOAT4 specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		};

		struct WVP
		{
			DirectX::XMFLOAT4X4 worldViewProjection;
		};

		struct Camera
		{
			DirectX::XMFLOAT3 cameraPosition;
			float pad;
		};

	}

}