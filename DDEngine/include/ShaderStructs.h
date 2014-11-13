#pragma once

#include <windows.h>
#include <string>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;

namespace DDEngine
{

	namespace Shaders {
		
		struct ShaderEnvelope
		{
			std::string name;
			std::string entryPoint;
			std::wstring path;
		};
	
		struct VertexShaderEnvelope : public ShaderEnvelope
		{
			LPVOID dataBlob = nullptr;
			DWORD dataSize = 0;
			ID3D11VertexShader* vertexShader;
		};

		struct PixelShaderEnvelope : public ShaderEnvelope
		{
			ID3D11PixelShader* pixelShader;
		};

		struct GeometryShaderEnvelope : public ShaderEnvelope
		{
			ID3D11GeometryShader* geometryShader;
		};

	}
}