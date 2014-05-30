#pragma once

#include <windows.h>
#include <string>
#include <d3d11.h>

namespace DDEngine
{

namespace ShaderCompiler
{

	struct ShaderCompilationResult {
		HRESULT result;
		std::string errorMessage;
	};

	ShaderCompilationResult compile( WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut );
	ShaderCompilationResult compile(LPVOID dataBlob, DWORD dataSize, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);

};
}