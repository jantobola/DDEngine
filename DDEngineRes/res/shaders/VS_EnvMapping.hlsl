
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

cbuffer Matrices : register ( b0 )
{
	matrix WVP;
};

struct VertexInput 
{
	float3 pos : SV_Position;
	float2 tex : TEXCOORD;
};

struct VertexOutput
{
	float4 pos : SV_Position;
	float3 tex : TEXCOORD;
};

VertexOutput main( VertexInput input )
{
	VertexOutput output;
	output.pos = mul(float4(input.pos, 1.0f), WVP).xyww;
	output.tex = input.pos;

	return output;
}