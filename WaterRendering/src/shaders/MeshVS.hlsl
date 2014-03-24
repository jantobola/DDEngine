
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

cbuffer PerObject : register ( b0 )
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

struct VertexOutpus 
{
    float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 nor : NORMAL;
};

VertexOutpus main( VertexInput input )
{
	VertexOutpus output;

	float4 pos = float4(input.pos.x, input.pos.y, input.pos.z, 1);

    output.pos = mul(pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.tex = input.tex;
	output.nor = input.nor;

	return output;
}