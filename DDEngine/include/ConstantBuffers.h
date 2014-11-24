#pragma once

#include "Material.h"
#include <windows.h>
#include <directxmath.h>

namespace DDEngine
{

	namespace CB
	{

		struct WorldViewProjection
		{
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 invTransWorld;
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

			Light() {
				for (size_t i = 1; i < Texture::NUM_TYPES; i++) {
					coords[i] = Texture::DEFAULT_COORDS;
				}
			}

			DirectX::XMFLOAT3 lightDirection = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			float pad0;
			DirectX::XMFLOAT3 lightPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			float pad1;
			DirectX::XMFLOAT3 eyePosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			float pad2;

			DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
			DirectX::XMFLOAT4 diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			DirectX::XMFLOAT4 specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			DirectX::XMINT4 enableDNSA = DirectX::XMINT4(0, 0, 0, 0);
			DirectX::XMINT4 enableEHSO = DirectX::XMINT4(0, 0, 0, 0);
			DirectX::XMINT3 enableDLR = DirectX::XMINT3(0, 0, 0);
			float shininess;

			DirectX::XMINT4 coords[Texture::NUM_TYPES];

			float attenuation = 1.0f;
			float spotAngle = 5.0f;
			float spotOuterAngle = 30.0f;

			void enableDiffuseTex() { enableDNSA.x = 1; }
			void disableDiffuseTex() { enableDNSA.x = 0; }

			void enableNormalMap() { enableDNSA.y = 1; }
			void disableNormalMap() { enableDNSA.y = 0; }

			void enableSpecularMap() { enableDNSA.z = 1; }
			void disableDiffuseMap() { enableDNSA.z = 0; }

			void enableAmbientMap() { enableDNSA.w = 1; }
			void disableAmbientMap() { enableDNSA.w = 0; }

		};

		struct Color
		{
			DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1, 1, 1, 1);
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