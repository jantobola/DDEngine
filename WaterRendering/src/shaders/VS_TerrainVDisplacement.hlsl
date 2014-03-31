
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

Texture2D<float> heightMapTexture : register( t0 );
SamplerState samplerState : register( s0 );

cbuffer Matrices : register ( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer TerrainProps : register ( b1 )
{
	float3 terrainSize;
	float2 textureSize;
}

struct VertexInput 
{
	float2 pos : POSITION;
};

struct VertexOutput 
{
    float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 nor : NORMAL;
};

// to CB -> terrain tweak bar
#define ELEVATION_FACTOR 9
#define SCALE_FACTOR 3
#define TEXTURE_SCALE_FACTOR 10

float3 elevate(float2 pos) {
	return (float3(pos, heightMapTexture.SampleLevel(samplerState, pos, 0) / ELEVATION_FACTOR) * SCALE_FACTOR).xzy;
}

VertexOutput main( VertexInput input )
{
	VertexOutput output;

	float2 position2 = input.pos;

	// get height for current vertex
	float4 position4 = float4(elevate(input.pos), 1);

	float3 xVec = elevate(position2 + float2(1.0 / (textureSize.x - 1), 0)) - elevate(position2 - float2(1.0 / (textureSize.x - 1), 0));
	float3 zVec = elevate(position2 + float2(0, 1.0 / (textureSize.y - 1))) - elevate(position2 - float2(0, 1.0 / (textureSize.y - 1)));

	// get cross product
	output.nor = cross(xVec, zVec);

    output.pos = mul(position4, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.tex = position2 * TEXTURE_SCALE_FACTOR;

	return output;
}