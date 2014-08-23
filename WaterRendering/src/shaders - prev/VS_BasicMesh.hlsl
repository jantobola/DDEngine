
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

cbuffer Matrices : register ( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInput 
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 nor : NORMAL;
};

struct VertexOutput 
{
    float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 nor : NORMAL;
};

VertexOutput main( VertexInput input )
{
	VertexOutput output;

	float4 pos = float4(input.pos, 1);

    output.pos = mul(pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.tex = input.tex;
	output.nor = input.nor;

	return output;
}