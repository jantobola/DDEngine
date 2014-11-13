
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------

cbuffer Matrices : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
	matrix invWorld;
    matrix invView;
    matrix invProjection;
	matrix scale;
}

struct GeometryInput
{
    float4 pos : SV_POSITION;
    float3 nor : NORMAL;
};

struct GeometryOutput
{
    float4 pos : SV_POSITION;
};

static float maxScale = 6;

[maxvertexcount(6)]
void main( triangle GeometryInput input[3], inout LineStream<GeometryOutput> lineStream )
{
    GeometryOutput output;

	for (uint i = 0; i < 3; i++) {
	
		float4 P = mul(mul(input[i].pos, invProjection), invView);

		float n1 = input[i].nor.x * min(scale._11, maxScale);
		float n2 = input[i].nor.y * min(scale._22, maxScale);
		float n3 = input[i].nor.z * min(scale._33, maxScale);

		float4 N = float4(P.xyz + float3(n1, n2, n3) * 0.2f, 1);

		output.pos = input[i].pos;
		lineStream.Append(output);

		output.pos = mul(mul(N, view), projection);
		lineStream.Append(output);

		lineStream.RestartStrip();
	}

	lineStream.RestartStrip();
}