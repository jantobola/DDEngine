#pragma once

#include <windows.h>
#include <string>
#include <d3d11.h>

namespace DDEngine
{

struct ShaderCompilationResult {
	HRESULT result;
	std::string errorMessage;
};

class ShaderCompiler {

	public:
		ShaderCompiler();
		~ShaderCompiler();

		static ShaderCompilationResult compile( WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut );

};
}