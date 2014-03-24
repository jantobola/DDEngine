
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

Texture2D<float> map : register( t0 );
SamplerState samp : register( s0 );

cbuffer PerObject : register ( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer HeightMap : register ( b1 )
{
	float3 terrainSize;
	float2 textureSize;
}

struct VertexInput 
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VertexOutput 
{
    float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 nor : NORMAL;
};

float3 elevate(float3 pos) {
	return (float3(pos.xz, map.SampleLevel(samp, pos.xz, 0)/ 9) * 3).xzy;
}

VertexOutput main( VertexInput input )
{
	VertexOutput output;
	input.pos.w = 1;
	// elevation

	float3 p = input.pos.xyz;
	input.pos = float4(elevate(input.pos.xyz), 1);

	float3 xVec = elevate(p + float3(1.0 / textureSize.x, 0, 0)) - elevate(p - float3(1.0 / textureSize.x, 0, 0));
	float3 zVec = elevate(p + float3(0, 0, 1.0 / textureSize.y)) - elevate(p - float3(0, 0, 1.0 / textureSize.y));

	output.nor = cross(xVec, zVec);

	// elevation

    output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.tex = float2(p.x * 10, p.z * 10);

	return output;
}