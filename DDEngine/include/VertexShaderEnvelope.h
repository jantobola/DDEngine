#pragma once

#include <string>

struct ID3D11VertexShader;

namespace DDEngine
{
struct VertexShaderEnvelope
{
	std::string name;
	std::wstring path;
	ID3D11VertexShader* vertexShader;
};
}