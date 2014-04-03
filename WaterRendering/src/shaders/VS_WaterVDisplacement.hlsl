
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> computedTexture : register( t0 );
SamplerState samplerState : register( s0 );

cbuffer Matrices : register ( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer Timer : register ( b1 )
{
	float time;
};

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

// GetDimensions
#define SIZE 100
#define HEIGHT_OFFSET -0.0001f

// input: pos.xy - 2D pixel coord
// output: pos.rgba - color components for a given pixel from computed texture
float4 getRGBA(float2 pos)
{
	return computedTexture.Load(int3(pos.x * SIZE, (1 - pos.y) * SIZE, 0));
}

// input: pos.xy - 2D grid coord
// output: pos.xyz - 3D coords containing height in pos.y
float3 getHeight(float2 pos)
{
	float height = getRGBA(pos).g; // height is in a G component
	return float3(pos, height).xzy;
}

VertexOutput main( VertexInput input )
{
	VertexOutput output;

	float4 rgba = getRGBA(input.pos);

	float2 position2 = input.pos;
	float4 position4 = float4(position2.x, rgba.g + HEIGHT_OFFSET, position2.y, 1);

	float3 xVec = getHeight(position2 + float2(1.0f / SIZE, 0)) - getHeight(position2 - float2(1.0f / SIZE, 0));
	float3 zVec = getHeight(position2 + float2(0, 1.0f / SIZE)) - getHeight(position2 - float2(0, 1.0f / SIZE));

	// get cross product
	output.nor = cross(xVec, zVec);
	output.tex = float2(position2.x, 1 - position2.y);
	
	//input.pos.y = sin(input.pos.y + velocity) / 2 * sin(input.pos.x + velocity);
	//input.pos = float4(input.pos.x, input.pos.y, input.pos.z, 1);

    output.pos = mul(position4, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	return output;
}