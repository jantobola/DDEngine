#pragma once

#include <windows.h>
#include <string>

struct ID3D11VertexShader;

namespace DDEngine
{

struct VertexShaderEnvelope
{
	std::string name;
	std::string entryPoint;
	std::wstring path;
	LPVOID dataBlob = nullptr;
	DWORD dataSize = 0;
	ID3D11VertexShader* vertexShader;
};
}