
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

struct VertexInput 
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VertexOutput 
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

VertexOutput main( VertexInput input )
{
	VertexOutput output;
	
	output.pos = float4(input.pos, 1);
	output.tex = input.tex;

	return output;
}