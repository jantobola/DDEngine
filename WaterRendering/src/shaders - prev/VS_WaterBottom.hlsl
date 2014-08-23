
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

cbuffer Matrices : register ( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer WaterProps : register ( b1 )
{
	float2 refractiveTextureScale : packoffset(c2.x);
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

#define TEXTURE_SCALE_FACTOR 6

VertexOutput main( VertexInput input )
{
	VertexOutput output;

	float4 position4 = float4(input.pos.x, 0, input.pos.y, 1);

    output.pos = mul(position4, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.tex = input.pos * refractiveTextureScale;
	
	// wrong light vectors, fix it later
	output.nor = float3(0, -1, 0);

	return output;
}