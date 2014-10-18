#include "D3DUtils.h"
#include "DDEUtils.h"
#include "ShaderCompiler.h"
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DDEngine;

HRESULT DDEngine::DXUtils::createDeviceAndSwapChain(_Out_ ID3D11Device** device, _Out_ ID3D11DeviceContext** context, _Out_ IDXGISwapChain** swapChain, _Out_ ID3D11RenderTargetView** renderTargetView, _In_ HWND hwnd, _In_ UINT multiSampling)
{
	HRESULT result = S_OK;
	UINT createDeviceFlags = 0;

	Dimension screenDimension = DDEUtils::getDimensionFromHandle(hwnd);

#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenDimension.WIDTH;
	swapChainDesc.BufferDesc.Height = screenDimension.HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = multiSampling;
	swapChainDesc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_QUALITY_LEVELS::D3D11_STANDARD_MULTISAMPLE_PATTERN;
	swapChainDesc.Windowed = true;

	D3D_FEATURE_LEVEL FEATURE_LEVEL;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
		D3D_DRIVER_TYPE DRIVER_TYPE = driverTypes[driverTypeIndex];

		result = D3D11CreateDeviceAndSwapChain(nullptr, DRIVER_TYPE, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, swapChain, device, &FEATURE_LEVEL, context);

		if (SUCCEEDED(result)) break;
	}

	if (*device && *context && *swapChain) {

		ID3D11Texture2D* backBuffer = NULL;
		result = (*swapChain)->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		HRESULT_RETURN_CHECK(result);

		result = (*device)->CreateRenderTargetView(backBuffer, NULL, renderTargetView);
		backBuffer->Release();
		HRESULT_RETURN_CHECK(result);

		(*context)->OMSetRenderTargets(1, renderTargetView, NULL);
	}

	return result;
}

HRESULT DDEngine::DXUtils::createRasterizerState(_In_ ID3D11Device* device, _Out_ ID3D11RasterizerState** rasterizerState, _In_ D3D11_CULL_MODE cullMode, _In_ D3D11_FILL_MODE fillMode, _In_ INT depthBias)
{
	HRESULT result = S_OK;
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = cullMode;
	rasterDesc.DepthBias = depthBias;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = fillMode;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterDesc, rasterizerState);
	return result;
}

HRESULT DDEngine::DXUtils::createAndCompileVertexShader(_In_ ID3D11Device* device, _In_ WCHAR* shaderName, _In_ LPCSTR entryPoint, _In_ LPCSTR shaderModel, _Out_ ID3D11VertexShader** vertexShader)
{
	HRESULT result = S_OK;

	ID3DBlob* vertexShaderBlob = NULL;
	ShaderCompiler::ShaderCompilationResult r = ShaderCompiler::compile(shaderName, entryPoint, shaderModel, &vertexShaderBlob);
	result = r.result;

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the shader compilation.\n\nCompiler message:\n" + r.errorMessage;
		Win32Utils::showFailMessage(result, "Vertex Shader Error", msg);
		return result;
	}

	result = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, vertexShader);

	if(vertexShaderBlob) vertexShaderBlob->Release();
	return result;
}

HRESULT DDEngine::DXUtils::createAndCompilePixelShader(_In_ ID3D11Device* device, _In_ WCHAR* shaderName, _In_ LPCSTR entryPoint, _In_ LPCSTR shaderModel, _Out_ ID3D11PixelShader** pixelShader)
{
	HRESULT result = S_OK;

	ID3DBlob* pixelShaderBlob = NULL;
	ShaderCompiler::ShaderCompilationResult r = ShaderCompiler::compile(shaderName, entryPoint, shaderModel, &pixelShaderBlob);
	result = r.result;

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the shader compilation.\n\nCompiler message:\n" + r.errorMessage;
		Win32Utils::showFailMessage(result, "Pixel Shader Error", msg);
		return result;
	}

	result = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, pixelShader);
	if(pixelShaderBlob) pixelShaderBlob->Release();

	return result;
}

HRESULT DDEngine::DXUtils::createVertexShaderFromMemory(_In_ ID3D11Device* device, _In_ LPVOID dataBlob, _In_ DWORD dataSize, _In_ LPCSTR entryPoint, _In_ LPCSTR shaderModel, _Out_ ID3D11VertexShader** vertexShader)
{
	HRESULT result = S_OK;

	ID3DBlob* vertexShaderBlob = NULL;
	ShaderCompiler::ShaderCompilationResult r = ShaderCompiler::compile(dataBlob, dataSize, entryPoint, shaderModel, &vertexShaderBlob);
	result = r.result;

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the shader compilation.\n\nCompiler message:\n" + r.errorMessage;
		Win32Utils::showFailMessage(result, "Vertex Shader Error", msg);
		return result;
	}

	result = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, vertexShader);

	if (vertexShaderBlob) vertexShaderBlob->Release();
	return result;
}

HRESULT DDEngine::DXUtils::createPixelShaderFromMemory(_In_ ID3D11Device* device, _In_ LPVOID dataBlob, _In_ DWORD dataSize, _In_ LPCSTR entryPoint, _In_ LPCSTR shaderModel, _Out_ ID3D11PixelShader** pixelShader)
{
	HRESULT result = S_OK;

	ID3DBlob* pixelShaderBlob = NULL;
	ShaderCompiler::ShaderCompilationResult r = ShaderCompiler::compile(dataBlob, dataSize, entryPoint, shaderModel, &pixelShaderBlob);
	result = r.result;

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the shader compilation.\n\nCompiler message:\n" + r.errorMessage;
		Win32Utils::showFailMessage(result, "Pixel Shader Error", msg);
		return result;
	}

	result = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, pixelShader);
	if (pixelShaderBlob) pixelShaderBlob->Release();

	return result;
}

HRESULT DDEngine::DXUtils::createVertexShaderFromBinary(_In_ ID3D11Device* device, _In_ WCHAR* shaderName, _Out_ ID3D11VertexShader** vertexShader)
{
	HRESULT result = S_OK;

	ID3DBlob* vertexShaderBlob = NULL;
	result = D3DReadFileToBlob(shaderName, &vertexShaderBlob);

	if (FAILED(result)) {
		Win32Utils::showFailMessage(result, "Vertex Shader Error", "Some errors occured during the loading shader from binary file.");
		return result;
	}

	result = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, vertexShader);

	if(vertexShaderBlob) vertexShaderBlob->Release();
	return result;
}

HRESULT DDEngine::DXUtils::createPixelShaderFromBinary(_In_ ID3D11Device* device, _In_ WCHAR* shaderName, _Out_ ID3D11PixelShader** pixelShader)
{
	HRESULT result = S_OK;

	ID3DBlob* pixelShaderBlob = NULL;
	result = D3DReadFileToBlob(shaderName, &pixelShaderBlob);

	if (FAILED(result)) {
		Win32Utils::showFailMessage(result, "Pixel Shader Error", "Some errors occured during the loading shader from binary file.");
		return result;
	}

	result = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, pixelShader);

	if(pixelShaderBlob) pixelShaderBlob->Release();
	return result;
}

HRESULT DDEngine::DXUtils::createIndexBuffer(_In_ ID3D11Device* device, _In_ std::vector<DWORD>* indices, _Out_ ID3D11Buffer** indexBuffer)
{
	HRESULT result = S_OK;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD)* indices->size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = &(*indices)[0];

	result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, indexBuffer);
	return result;
}

// not good to have a hardcoded vertex type...
HRESULT DDEngine::DXUtils::createVertexBuffer(_In_ ID3D11Device* device, _In_ std::vector<Vertex>* vertices, _Out_ ID3D11Buffer** vertexBuffer)
{
	HRESULT result = S_OK;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* vertices->size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &(*vertices)[0];

	result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, vertexBuffer);
	return result;
}

HRESULT DDEngine::DXUtils::createDepthStencilBuffer(_In_ ID3D11Device* device, _In_ ID3D11DeviceContext* context, _In_ Dimension screenDimension, _Out_ ID3D11DepthStencilView** depthStencilView, _Out_ ID3D11Texture2D** depthStencilBuffer, _In_	UINT multiSampling)
{
	HRESULT result = S_OK;
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = screenDimension.WIDTH;
	depthStencilDesc.Height = screenDimension.HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = multiSampling;
	depthStencilDesc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_QUALITY_LEVELS::D3D11_STANDARD_MULTISAMPLE_PATTERN;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthStencilDesc, NULL, depthStencilBuffer);
	HRESULT_RETURN_CHECK(result)

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	if(multiSampling > 1) {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	} else {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	}
	
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(*depthStencilBuffer, &depthStencilViewDesc, depthStencilView);
	return result;
}

HRESULT DDEngine::DXUtils::createConstatnBuffer(_In_ ID3D11Device* device, _In_ UINT byteWidth, _Out_ ID3D11Buffer** constantBuffer)
{
	HRESULT result = S_OK;

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = byteWidth;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;

	result = device->CreateBuffer(&constantBufferDesc, NULL, constantBuffer);
	return result;
}

HRESULT DDEngine::DXUtils::createInputLayout(_In_ ID3D11Device* device, _In_ WCHAR* shaderName, _In_ LPCSTR entryPoint, _In_ LPCSTR shaderModel, _Out_ ID3D11InputLayout** inputLayout, _In_ D3D11_INPUT_ELEMENT_DESC layoutDesc[], _In_ UINT numElements)
{
	HRESULT result = S_OK;
	ID3DBlob* blob = nullptr;
	ShaderCompiler::ShaderCompilationResult r = ShaderCompiler::compile(shaderName, entryPoint, shaderModel, &blob);
	result = r.result;

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the shader compilation.\n\nCompiler message:\n" + r.errorMessage;
		Win32Utils::showFailMessage(result, "Input Layout Error", msg);

		if (blob) blob->Release();
		return result;
	}

	result = device->CreateInputLayout(layoutDesc, numElements, blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout);

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the creating an input layout.";
		Win32Utils::showFailMessage(result, "Input Layout Error", msg);
	}

	if (blob) blob->Release();
	return result;
}

HRESULT DDEngine::DXUtils::createInputLayoutFromMemory(_In_ ID3D11Device* device, _In_ LPVOID dataBlob, _In_ DWORD dataSize, _In_ LPCSTR entryPoint, _In_ LPCSTR shaderModel, _Out_ ID3D11InputLayout** inputLayout, _In_ D3D11_INPUT_ELEMENT_DESC layoutDesc[], _In_ UINT numElements)
{
	HRESULT result = S_OK;
	ID3DBlob* blob = nullptr;
	ShaderCompiler::ShaderCompilationResult r = ShaderCompiler::compile(dataBlob, dataSize, entryPoint, shaderModel, &blob);
	result = r.result;

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the shader compilation.\n\nCompiler message:\n" + r.errorMessage;
		Win32Utils::showFailMessage(result, "Input Layout Error", msg);

		if (blob) blob->Release();
		return result;
	}

	result = device->CreateInputLayout(layoutDesc, numElements, blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout);

	if (FAILED(result)) {
		std::string msg = "Some errors occured during the creating an input layout.";
		Win32Utils::showFailMessage(result, "Input Layout Error", msg);
	}

	if (blob) blob->Release();
	return result;
}

HRESULT DDEngine::DXUtils::createInputLayoutFromBinary(_In_ ID3D11Device* device, _In_ WCHAR* shaderName, _Out_ ID3D11InputLayout** inputLayout, _In_ D3D11_INPUT_ELEMENT_DESC* layoutDesc, _In_ UINT numElements)
{
	HRESULT result = S_OK;
	ID3DBlob* blob = NULL;
	HRESULT r = D3DReadFileToBlob(shaderName, &blob);

	HRESULT_RETURN_CHECK(result)

	result = device->CreateInputLayout(layoutDesc, numElements, blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout);
	blob->Release();

	return result;
}

HRESULT DDEngine::DXUtils::createSamplerState(_In_ ID3D11Device* device, _Out_ ID3D11SamplerState** samplerState, _In_opt_ D3D11_SAMPLER_DESC* samplerDesc /*= NULL */)
{
	if (samplerDesc != NULL) {
		return device->CreateSamplerState(samplerDesc, samplerState);
	}

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// default value point sampler
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.MaxAnisotropy = 16;
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 1;
	desc.BorderColor[2] = 1;
	desc.BorderColor[3] = 1;

	return device->CreateSamplerState(&desc, samplerState);
}

HRESULT DDEngine::DXUtils::createSamplerState(_In_ ID3D11Device* device, _Out_ ID3D11SamplerState** samplerState, _In_ FilterType filter, _In_ TextureAddressMode textureMode, _In_ ComparisonFunction comparison, _In_opt_ UINT maxAnisotropy)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = filter;
	desc.AddressU = textureMode;
	desc.AddressV = textureMode;
	desc.AddressW = textureMode;
	desc.ComparisonFunc = comparison;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.MaxAnisotropy = maxAnisotropy;
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 1;
	desc.BorderColor[2] = 1;
	desc.BorderColor[3] = 1;
	
	return DXUtils::createSamplerState(device, samplerState, &desc);
}

HRESULT DDEngine::DXUtils::createBlendState(_In_ ID3D11Device* device, _Out_ ID3D11BlendState** blendState, _In_ D3D11_BLEND_DESC* blendDesc /*= NULL */)
{
	if (blendDesc != NULL) {
		return device->CreateBlendState(blendDesc, blendState);
	}

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// ALPHA BLEND BY DEFAULT

	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return device->CreateBlendState(&desc, blendState);
}

HRESULT DDEngine::DXUtils::createCubeTextureResource(_In_ ID3D11Device* device, _In_ LPCWSTR path, _Out_ ID3D11ShaderResourceView** texture)
{
	ID3D11Texture2D* SMTexture = NULL;
	HRESULT result = S_OK;

	result = DirectX::CreateDDSTextureFromFile(device, path, (ID3D11Resource**) &SMTexture, nullptr);

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(SMTexture, &SMViewDesc, texture);
	if(SMTexture) SMTexture->Release();

	return result;
}