#pragma once

#include "VertexShaderEnvelope.h"
#include "PixelShaderEnvelope.h"
#include <d3d11.h>
#include <unordered_map>

namespace DDEngine
{

class RenderContext;
class Config;

enum ShaderType
{
	VS, PS, CS, GS, HS, DS
};

class ShaderHolder {

	typedef std::string string;
	typedef std::wstring wstring;

	private:
		typedef std::unordered_map<std::string, VertexShaderEnvelope> VertexShaders;
		typedef std::unordered_map<std::string, PixelShaderEnvelope> PixelShaders;
		typedef std::unordered_map<std::string, ID3D11Buffer*> ConstantBuffers;
		typedef std::unordered_map<std::string, ID3D11InputLayout*> InputLayouts;

		VertexShaders vertexShaders;
		PixelShaders pixelShaders;
		ConstantBuffers constantBuffers;
		InputLayouts inputLayouts;

		Config& config;
		RenderContext& renderContext;

		string activeVS;
		string activePS;
		string activeIL;

		FILETIME lastFileTime;

		string compileListenOn;
		ShaderHolder::ShaderType compileListenOnType;
		bool isRealTimeCompilerRunning;
		bool isAllCompile;

	public:
		ShaderHolder(Config& config, RenderContext& renderContext);
		~ShaderHolder();

		void load();

		void addVertexShader(string name, wstring path, string shaderModel, string entryPoint = "main");
		void addPixelShader(string name, wstring path, string shaderModel, string entryPoint = "main");
		void addVertexShaderFromMemory(string name, LPVOID dataBlob, DWORD dataSize, string shaderModel, string entryPoint = "main");
		void addPixelShaderFromMemory(string name, LPVOID dataBlob, DWORD dataSize, string shaderModel, string entryPoint = "main");
		void addVertexShaderBinary(string name, wstring path);
		void addPixelShaderBinary(string name, wstring path);
		void addConstantBuffer(string name, UINT byteWidth);
		void addInputLayout(string name, string shaderName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements, string entryPoint = "main");
		void addInputLayoutBinary(string name, string shaderName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);

		void recompileVertexShader(string name);
		void recompilePixelShader(string name);

		void runRealTimeCompiler(string name, string type);
		void runRealTimeCompilerAll();
		void stopRealTimeCompiler();

		void realTimeCompile();

		string getActiveVS();
		string getActivePS();
		string getActiveIL();

		ID3D11VertexShader* getVertexShader(string name);
		ID3D11PixelShader* getPixelShader(string name);
		ID3D11Buffer* getConstatnBuffer(string name);
		ID3D11InputLayout* getInputLayout(string name);

		void activateVS(string name);
		void activatePS(string name);
		void activateIL(string name);

		void updateConstantBufferVS( string name, const void* bufferData, UINT startSlot );
		void updateConstantBufferPS( string name, const void* bufferData, UINT startSlot );

		void refreshShaders();
		void cleanUp();
};
}