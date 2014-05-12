#include "ShaderCompiler.h"
#include <d3dcompiler.h>
#include <d3d11.h>

using namespace DDEngine;
using namespace ShaderCompiler;

ShaderCompilationResult ShaderCompiler::compile( WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut ) {
	HRESULT hr = S_OK;
	ShaderCompilationResult result;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#else
		dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
	#endif
		
	ID3DBlob* pErrorBlob = NULL;
	hr = D3DCompileFromFile(fileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob );

	result.result = hr;

	if( FAILED(hr) ) {
		if( pErrorBlob != NULL ) {
			std::string s = std::string((char*)pErrorBlob->GetBufferPointer());
			result.errorMessage = s;
		}
	}

	if( pErrorBlob ) pErrorBlob->Release();
	return result;
}
