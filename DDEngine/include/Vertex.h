#pragma once

#include <directxmath.h>

/*
	Deprecated, use DirectXTK/VertexTypes.h instead.	
*/


namespace DDEngine
{
struct VertexPosTexNor
{
	VertexPosTexNor() {

	}

	VertexPosTexNor(
		float x,	float y,	float z,
		float u,	float v,
		float nx,	float ny,	float nz
	) : 
		pos			(x,		y,		z),
		texCoord	(u,		v),
		normal		(nx,	ny,		nz)
	{ }

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
};

}