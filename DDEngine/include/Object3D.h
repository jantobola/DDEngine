#pragma once

#include "Vertex.h"
#include <vector>
#include <windows.h>
#include <DirectXMath.h>
#include <d3d11.h>

namespace DDEngine
{
class Object3D {
		
	typedef std::string string;

	private:
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;

	protected:
		ID3D11Buffer *vertexBuffer, *indexBuffer;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;

		int verticesSize;
		int indicesSize;
		bool isHidden;

		string vertexShaderName;
		string pixelShaderName;
		string inputLayoutName;

	public:
		Object3D();
		virtual ~Object3D();

		enum PrimitiveTopology {
			TRIANGLE_LIST,
			TRIANGLE_STRIP,
		};

		HRESULT registerObject(ID3D11Device* device, ID3D11DeviceContext* context);
		void draw();

		int getIndexCount();
		int getVertexCount();

		void setShaders( string vs, string ps, string il);
		void setPrimitiveTopology(Object3D::PrimitiveTopology primitiveTopology);

		string getVSName();
		string getPSName();
		string getILName();

		void hide();
		void show();

		DirectX::XMMATRIX getWorldMatrix();

		// TRANSFORMATIONS

		void rotateX(float x);
		void rotateY(float y);
		void rotateZ(float z);
		void rotate(float x, float y, float z);
		void translate(float x, float y, float z);
		void scale(float x, float y, float z);
		void scale(float scale);

		void releaseBuffers();

	protected:

		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 rotationMatrix;
		DirectX::XMFLOAT4X4 translationMatrix;
		DirectX::XMFLOAT4X4 scaleMatrix;
		
		void VB(float x, float y, float z, float u, float v, float nx, float ny, float nz);
		void IB(int index);
		virtual void loadGeometry() = 0;
};
}