
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

cbuffer Matrices : register ( b0 )
{
	matrix WVP;
};

struct VertexInput 
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VertexOutpus 
{
    float4 pos : SV_POSITION;
	float3 tex : TEXCOORD;
};

VertexOutpus main( VertexInput input )
{
	VertexOutpus output;
	output.pos = mul(float4(input.pos, 1.0f), WVP).xyww;
	output.tex = input.pos;

	return output;
}