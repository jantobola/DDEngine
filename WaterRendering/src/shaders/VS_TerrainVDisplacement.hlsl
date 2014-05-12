
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> heightMapTexture : register( t0 );
SamplerState samplerState : register( s0 );

cbuffer Matrices : register ( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer TerrainProps : register ( b1 )
{	
	//c0
	float2 textureSize;
	float elevationFactor;
	float scaleFactor;
	
	//c1
	float textureScaleFactor;
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

float3 elevate(float2 pos) {
	return (float3(pos, heightMapTexture.SampleLevel(samplerState, float2(pos.x, 1 - pos.y), 0).r * elevationFactor) * scaleFactor).xzy;
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

	output.tex = position2 * textureScaleFactor;

	return output;
}