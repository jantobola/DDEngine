
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> previousFrame : register( t0 );
SamplerState samp : register( s0 );

cbuffer PerObject : register ( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer Timer : register ( b1 )
{
	float velocity;
};

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

#define SIZE 400

float3 getHeight(float3 pos)
{
	//float height = previousFrame.SampleLevel(samp, float2(pos.x, pos.z), 0).g;
	float height = previousFrame.Load(int3(pos.x * SIZE, (1 - pos.z) * SIZE, 0)).g;
	return float3(pos.x, height, pos.z);
}


VertexOutput main( VertexInput input )
{
	VertexOutput output;
	input.pos.w = 1;
	
	//float4 rgba = previousFrame.SampleLevel(samp, float2(input.pos.x, 1 - input.pos.z), 0);
	float4 rgba = previousFrame.Load(int3(input.pos.x * SIZE, (1 - input.pos.z) * SIZE, 0));
	input.pos.y = rgba.g;

	float3 p = float3(input.pos.xyz);

		float3 xVec = getHeight(p + float3(1.0f / SIZE, 0, 0)) - getHeight(p - float3(1.0f / SIZE, 0, 0));
		float3 zVec = getHeight(p + float3(0, 0, 1.0f / SIZE)) - getHeight(p - float3(0, 0, 1.0f / SIZE));

	output.nor = cross(xVec, zVec);
	output.tex = float2(input.pos.x, 1 - input.pos.z);
	
	//input.pos.y = sin(input.pos.y + velocity) / 2 * sin(input.pos.x + velocity);
	//input.pos = float4(input.pos.x, input.pos.y, input.pos.z, 1);

    output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	return output;
}