#include "ShaderHolder.h"
#include "RenderContext.h"
#include "Config.h"
#include "D3DUtils.h"
#include "DDEUtils.h"
#include "DLLResourceLoader.h"
#include <DirectXTK/VertexTypes.h>

using namespace DDEngine;
using namespace Shaders;
using namespace DirectX;

ShaderHolder::ShaderHolder(Config& config, RenderContext& renderContext) : config(config), renderContext(renderContext) {
	this->config = config;
	this->isRealTimeCompilerRunning = false;
	this->compileListenOnType = ShaderType::VS;
	this->isAllCompile = false;

	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	SystemTimeToFileTime(&sysTime, &(this->lastFileTime));
}

ShaderHolder::~ShaderHolder() {

}

void ShaderHolder::load() {

	// Here you can load engine built-in shaders

	HMODULE dll = LoadLibrary(L"dderes.dll");

	DataContainer VS_Skybox = DLLResourceLoader::loadFromDLL(dll, L"SHADERS", 201);
	DataContainer PS_Skybox = DLLResourceLoader::loadFromDLL(dll, L"SHADERS", 202);
	DataContainer PS_Selection = DLLResourceLoader::loadFromDLL(dll, L"SHADERS", 203);
	DataContainer PS_NormalVisualizer = DLLResourceLoader::loadFromDLL(dll, L"SHADERS", 204);
	DataContainer VS_IL = DLLResourceLoader::loadFromDLL(dll, L"SHADERS", 205);
	DataContainer GS_NormalVisualizer = DLLResourceLoader::loadFromDLL(dll, L"SHADERS", 206);

	addVertexShaderFromMemory("DDEngine_VS_Skybox", VS_Skybox.dataBlob, VS_Skybox.dataSize, "vs_4_0");
	addPixelShaderFromMemory("DDEngine_PS_Skybox", PS_Skybox.dataBlob, PS_Skybox.dataSize, "ps_4_0");
	addPixelShaderFromMemory("DDEngine_PS_Selection", PS_Selection.dataBlob, PS_Selection.dataSize, "ps_4_0");
	addPixelShaderFromMemory("DDEngine_PS_NormalVisualizer", PS_NormalVisualizer.dataBlob, PS_NormalVisualizer.dataSize, "ps_4_0");
	addVertexShaderFromMemory("DDEngine_VS_IL", VS_IL.dataBlob, VS_IL.dataSize, "vs_4_0");

	addInputLayout("POS_TEX", "DDEngine_VS_IL", VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount, "POS_TEX");
	addInputLayout("POS_NOR", "DDEngine_VS_IL", VertexPositionNormal::InputElements, VertexPositionNormal::InputElementCount, "POS_NOR");
	addInputLayout("POS_COL", "DDEngine_VS_IL", VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, "POS_COL");

	addInputLayout("POS_NOR_TEX", "DDEngine_VS_IL", VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount, "POS_NOR_TEX");
	addInputLayout("POS_COL_TEX", "DDEngine_VS_IL", VertexPositionColorTexture::InputElements, VertexPositionColorTexture::InputElementCount, "POS_COL_TEX");
	addInputLayout("POS_NOR_COL", "DDEngine_VS_IL", VertexPositionNormalColor::InputElements, VertexPositionNormalColor::InputElementCount, "POS_NOR_COL");

	addInputLayout("POS_NOR_COL_TEX", "DDEngine_VS_IL", VertexPositionNormalColorTexture::InputElements, VertexPositionNormalColorTexture::InputElementCount, "POS_NOR_COL_TEX");
	addInputLayout("POS_NOR_TAN_COL_TEX", "DDEngine_VS_IL", VertexPositionNormalTangentColorTexture::InputElements, VertexPositionNormalTangentColorTexture::InputElementCount, "POS_NOR_TAN_COL_TEX");

	addConstantBuffer("DDEngine_WVP", sizeof(CB::WVP));
	addConstantBuffer("DDEngine_Matrices", sizeof(CB::GSMatrices));
	addConstantBuffer("DDEngine_Color", sizeof(CB::Color));

	// unused
	D3D11_INPUT_ELEMENT_DESC pos_nor_tan_tex[] = {
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_SO_DECLARATION_ENTRY gsLayout[] =
	{
		{ 0, "SV_POSITION", 0, 0, 4, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};

	addInputLayout("POS_NOR_TAN_TEX", "DDEngine_VS_IL", pos_nor_tan_tex, ARRAYSIZE(pos_nor_tan_tex), "POS_NOR_TAN_TEX");
	addGeometryShaderFromMemory("DDEngine_GS_NormalVisualizer", GS_NormalVisualizer.dataBlob, GS_NormalVisualizer.dataSize, "", gsLayout, ARRAYSIZE(gsLayout));

	FreeLibrary(dll);
}

void DDEngine::ShaderHolder::addVertexShader(string name, wstring path, string shaderModel, string entryPoint /*= "main"*/)
{
	string model = (shaderModel.empty()) ? config.CFG_VERTEX_SHADER_MODEL : shaderModel;

	ID3D11VertexShader* vs;
	HRESULT result = DXUtils::createAndCompileVertexShader(renderContext.device, &path[0], entryPoint.c_str(), model.c_str(), &vs);

	if (!FAILED(result)) {
		VertexShaderEnvelope envelope;
		envelope.name = name;
		envelope.entryPoint = entryPoint;
		envelope.path = path;
		envelope.vertexShader = vs;
		vertexShaders.insert(VertexShaders::value_type(name, envelope));
	}
}

void DDEngine::ShaderHolder::addPixelShader(string name, wstring path, string shaderModel, string entryPoint /*= "main"*/)
{
	string model = (shaderModel.empty()) ? config.CFG_PIXEL_SHADER_MODEL : shaderModel;

	ID3D11PixelShader* ps;
	HRESULT result = DXUtils::createAndCompilePixelShader(renderContext.device, &path[0], entryPoint.c_str(), model.c_str(), &ps);

	if (!FAILED(result)) {
		PixelShaderEnvelope envelope;
		envelope.name = name;
		envelope.entryPoint = entryPoint;
		envelope.path = path;
		envelope.pixelShader = ps;
		pixelShaders.insert(PixelShaders::value_type(name, envelope));
	}
}

void DDEngine::ShaderHolder::addGeometryShader(string name, wstring path, string shaderModel, const D3D11_SO_DECLARATION_ENTRY* layoutDesc, UINT numElements, string entryPoint /*= "main"*/)
{
	string model = (shaderModel.empty()) ? config.CFG_GEOMETRY_SHADER_MODEL : shaderModel;

	ID3D11GeometryShader* gs;
	HRESULT result = DXUtils::createAndCompileGeometryShader(renderContext.device, &path[0], entryPoint.c_str(), model.c_str(), &gs, layoutDesc, numElements);

	if (!FAILED(result)) {
		GeometryShaderEnvelope envelope;
		envelope.name = name;
		envelope.entryPoint = entryPoint;
		envelope.path = path;
		envelope.geometryShader = gs;
		geometryShaders.insert(GeometryShaders::value_type(name, envelope));
	}
}

void DDEngine::ShaderHolder::addVertexShaderFromMemory(string name, LPVOID dataBlob, DWORD dataSize, string shaderModel, string entryPoint /*= "main"*/)
{
	string model = (shaderModel.empty()) ? config.CFG_VERTEX_SHADER_MODEL : shaderModel;

	ID3D11VertexShader* vs;
	HRESULT result = DXUtils::createVertexShaderFromMemory(renderContext.device, dataBlob, dataSize, entryPoint.c_str(), model.c_str(), &vs);

	if (!FAILED(result)) {
		VertexShaderEnvelope envelope;
		envelope.name = name;
		envelope.entryPoint = entryPoint;
		envelope.dataBlob = dataBlob;
		envelope.dataSize = dataSize;
		envelope.vertexShader = vs;
		vertexShaders.insert(VertexShaders::value_type(name, envelope));
	}
}

void DDEngine::ShaderHolder::addPixelShaderFromMemory(string name, LPVOID dataBlob, DWORD dataSize, string shaderModel, string entryPoint /*= "main"*/)
{
	string model = (shaderModel.empty()) ? config.CFG_PIXEL_SHADER_MODEL : shaderModel;

	ID3D11PixelShader* ps;
	HRESULT result = DXUtils::createPixelShaderFromMemory(renderContext.device, dataBlob, dataSize, entryPoint.c_str(), model.c_str(), &ps);

	if (!FAILED(result)) {
		PixelShaderEnvelope envelope;
		envelope.name = name;
		envelope.entryPoint = entryPoint;
		envelope.pixelShader = ps;
		pixelShaders.insert(PixelShaders::value_type(name, envelope));
	}
}


void DDEngine::ShaderHolder::addGeometryShaderFromMemory(string name, LPVOID dataBlob, DWORD dataSize, string shaderModel, const D3D11_SO_DECLARATION_ENTRY* layoutDesc, UINT numElements, string entryPoint /*= "main"*/) {
	string model = (shaderModel.empty()) ? config.CFG_GEOMETRY_SHADER_MODEL : shaderModel;

	ID3D11GeometryShader* gs;
	HRESULT result = DXUtils::createGeometryShaderFromMemory(renderContext.device, dataBlob, dataSize, entryPoint.c_str(), model.c_str(), &gs, layoutDesc, numElements);

	if (!FAILED(result)) {
		GeometryShaderEnvelope envelope;
		envelope.name = name;
		envelope.entryPoint = entryPoint;
		envelope.path = L"";
		envelope.geometryShader = gs;
		geometryShaders.insert(GeometryShaders::value_type(name, envelope));
	}
}


void ShaderHolder::addVertexShaderBinary(string name, wstring path) {
	ID3D11VertexShader* vs;
	HRESULT result = DXUtils::createVertexShaderFromBinary(renderContext.device, &path[0], &vs);
	
	if (!FAILED(result)) {
		VertexShaderEnvelope envelope;
		envelope.name = name;
		envelope.path = path;
		envelope.vertexShader = vs;
		vertexShaders.insert(VertexShaders::value_type(name, envelope));
	}
}

void ShaderHolder::addPixelShaderBinary( string name, wstring path ) {
	ID3D11PixelShader* ps;
	HRESULT result = DXUtils::createPixelShaderFromBinary(renderContext.device, &path[0], &ps);

	if (!FAILED(result)) {
		PixelShaderEnvelope envelope;
		envelope.name = name;
		envelope.path = path;
		envelope.pixelShader = ps;
		pixelShaders.insert(PixelShaders::value_type(name, envelope));
	}
}

void ShaderHolder::addInputLayout(string name, string shaderName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements, string entryPoint) {
	ID3D11InputLayout* inputLayout;
	wstring path;
	HRESULT result = S_OK;

	VertexShaders::iterator it = vertexShaders.find(shaderName);
	if(it != vertexShaders.end()) {
		path = it->second.path;
	}

	if (!path.empty()) {
		result = DXUtils::createInputLayout(renderContext.device, &path[0], entryPoint.c_str(), config.CFG_VERTEX_SHADER_MODEL.c_str(), &inputLayout, layout, numElements);
	}
	else {
		result = DXUtils::createInputLayoutFromMemory(renderContext.device, it->second.dataBlob, it->second.dataSize, entryPoint.c_str(), config.CFG_VERTEX_SHADER_MODEL.c_str(), &inputLayout, layout, numElements);
	}

	if (!FAILED(result)) {
		inputLayouts.insert(InputLayouts::value_type(name, inputLayout));
	}
}

void ShaderHolder::addInputLayoutBinary(string name, string shaderName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements) {
	ID3D11InputLayout* inputLayout;
	wstring path;

	VertexShaders::iterator it = vertexShaders.find(shaderName);
	if(it != vertexShaders.end()) {
		path = it->second.path;
	}

	if (path.empty()) return;
	HRESULT result = S_OK;
	
	result = DXUtils::createInputLayoutFromBinary(renderContext.device, &path[0], &inputLayout, layout, numElements);

	if (!FAILED(result)) {
		inputLayouts.insert(InputLayouts::value_type(name, inputLayout));
	}
}

void ShaderHolder::addConstantBuffer( string name, UINT byteWidth ) {
	ID3D11Buffer* constantBuffer;
	HRESULT result = DXUtils::createConstatnBuffer(renderContext.device, byteWidth, &constantBuffer);

	if (!FAILED(result)) {
		constantBuffers.insert(ConstantBuffers::value_type(name, constantBuffer));
	}
}

void ShaderHolder::cleanUp() {
	for (VertexShaders::iterator it = vertexShaders.begin(); it != vertexShaders.end(); ++it) {
		it->second.vertexShader->Release();
	}

	for (PixelShaders::iterator it = pixelShaders.begin(); it != pixelShaders.end(); ++it) {
		it->second.pixelShader->Release();
	}

	for (GeometryShaders::iterator it = geometryShaders.begin(); it != geometryShaders.end(); ++it) {
		it->second.geometryShader->Release();
	}

	for (InputLayouts::iterator it = inputLayouts.begin(); it != inputLayouts.end(); ++it) {
		it->second->Release();
	}

	for (ConstantBuffers::iterator it = constantBuffers.begin(); it != constantBuffers.end(); ++it) {
		it->second->Release();
	}
}

ID3D11VertexShader* ShaderHolder::getVertexShader( string name ) {
	VertexShaders::iterator it = vertexShaders.find(name);
	if(it != vertexShaders.end()) {
		return it->second.vertexShader;
	}

	return NULL;
}

ID3D11PixelShader* ShaderHolder::getPixelShader( string name ) {
	PixelShaders::iterator it = pixelShaders.find(name);
	if(it != pixelShaders.end()) {
		return it->second.pixelShader;
	}

	return NULL;
}

ID3D11GeometryShader* DDEngine::ShaderHolder::getGeometryShader(string name)
{
	GeometryShaders::iterator it = geometryShaders.find(name);
	if (it != geometryShaders.end()) {
		return it->second.geometryShader;
	}

	return NULL;
}

ID3D11Buffer* ShaderHolder::getConstatnBuffer( string name )
{
	ConstantBuffers::iterator it = constantBuffers.find(name);
	if(it != constantBuffers.end()) {
		return it->second;
	}

	return NULL;
}

ID3D11InputLayout* ShaderHolder::getInputLayout( string name )
{
	InputLayouts::iterator it = inputLayouts.find(name);
	if(it != inputLayouts.end()) {
		return it->second;
	}

	return NULL;
}

void ShaderHolder::activateVS( string name ) {
	renderContext.context->VSSetShader(getVertexShader(name), NULL, 0);
	activeVS = name;
}

void ShaderHolder::activatePS( string name ) {
	renderContext.context->PSSetShader(getPixelShader(name), NULL, 0);
	activePS = name;
}

void DDEngine::ShaderHolder::activateGS(string name)
{
	renderContext.context->GSSetShader(getGeometryShader(name), NULL, 0);
	activeGS = name;
}

void ShaderHolder::activateIL( string name ) {
		renderContext.context->IASetInputLayout(getInputLayout(name));
		activeIL = name;
}

void ShaderHolder::updateConstantBufferVS( string name, const void* bufferData, UINT startSlot ) {
	ID3D11Buffer* constantBuffer = getConstatnBuffer(name);
	renderContext.context->UpdateSubresource(constantBuffer, 0, NULL, bufferData, 0, 0);
	renderContext.context->VSSetConstantBuffers( startSlot, 1, &constantBuffer );
}

void ShaderHolder::updateConstantBufferPS( string name, const void* bufferData, UINT startSlot ) {
	ID3D11Buffer* constantBuffer = getConstatnBuffer(name);
	renderContext.context->UpdateSubresource(constantBuffer, 0, NULL, bufferData, 0, 0);
	renderContext.context->PSSetConstantBuffers( startSlot, 1, &constantBuffer );
}

void ShaderHolder::updateConstantBufferGS( string name, const void* bufferData, UINT startSlot ) {
	ID3D11Buffer* constantBuffer = getConstatnBuffer(name);
	renderContext.context->UpdateSubresource(constantBuffer, 0, NULL, bufferData, 0, 0);
	renderContext.context->GSSetConstantBuffers(startSlot, 1, &constantBuffer);
}

std::string ShaderHolder::getActiveVS() {
	return this->activeVS;
}

std::string ShaderHolder::getActivePS() {
	return this->activePS;
}

std::string ShaderHolder::getActiveIL() {
	return this->activeIL;
}

void ShaderHolder::refreshShaders() {
	activateVS(activeVS);
	activatePS(activePS);
	activateIL(activeIL);
}

void ShaderHolder::recompileVertexShader( string name ) {
	VertexShaders::iterator it = vertexShaders.find(name);
	if(it != vertexShaders.end()) {
		ID3D11VertexShader* vs;
		
		if (it->second.path.empty()) return;
		if (!FileUtils::isFileLocked(it->second.path) && (CompareFileTime(&FileUtils::getLastWriteTime(it->second.path), &this->lastFileTime) == 1)) {
	
			HRESULT result = DXUtils::createAndCompileVertexShader(renderContext.device, &it->second.path[0], 
				it->second.entryPoint.c_str(), config.CFG_VERTEX_SHADER_MODEL.c_str(), &vs);

			if (!FAILED(result)) {
				ID3D11VertexShader* old = it->second.vertexShader;
				it->second.vertexShader = vs;
				old->Release();
				this->lastFileTime = FileUtils::getLastWriteTime(it->second.path);
			}

		}
	}
}

void ShaderHolder::recompilePixelShader( string name ) {
	PixelShaders::iterator it = pixelShaders.find(name);
	if(it != pixelShaders.end()) {
		ID3D11PixelShader* ps;

		if (it->second.path.empty()) return;
		if (!FileUtils::isFileLocked(it->second.path) && (CompareFileTime(&FileUtils::getLastWriteTime(it->second.path), &this->lastFileTime) == 1)) {
			this->lastFileTime = FileUtils::getLastWriteTime(it->second.path);
			
			HRESULT result = DXUtils::createAndCompilePixelShader(renderContext.device, &it->second.path[0], 
				it->second.entryPoint.c_str(), config.CFG_PIXEL_SHADER_MODEL.c_str(), &ps);
		
			if (!FAILED(result)) {
				ID3D11PixelShader* old = it->second.pixelShader;
				it->second.pixelShader = ps;
				old->Release();
			}
		}
	}
}

void ShaderHolder::realTimeCompile() {
	if (isAllCompile) {
		
		for (VertexShaders::iterator it = vertexShaders.begin(); it != vertexShaders.end(); ++it) {
			recompileVertexShader(it->first);
		}

		for (PixelShaders::iterator it = pixelShaders.begin(); it != pixelShaders.end(); ++it) {
			recompilePixelShader(it->first);
		}

	} else if (isRealTimeCompilerRunning) {
		switch (compileListenOnType) {
			
			case VS:
				recompileVertexShader(compileListenOn);
				break;
			
			case PS:
				recompilePixelShader(compileListenOn);
				break;
			
		}	
	}
}

void ShaderHolder::runRealTimeCompiler(string name, string type) {
	this->compileListenOn = name;
	this->isRealTimeCompilerRunning = true;
	if(type == "VS") this->compileListenOnType = ShaderType::VS;
	if(type == "PS") this->compileListenOnType = ShaderType::PS;
}

void ShaderHolder::stopRealTimeCompiler() {
	this->isRealTimeCompilerRunning = false;
	this->isAllCompile = false;
}

void ShaderHolder::runRealTimeCompilerAll() {
	this->isAllCompile = true;
}