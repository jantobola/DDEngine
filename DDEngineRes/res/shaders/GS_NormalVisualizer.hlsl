
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------

cbuffer Matrices : register(b0)
{
    matrix world;
	matrix invTransWorld;
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
	//float3 tan : TANGENT;
	//float3 bitan : BINORMAL;
};

struct GeometryOutput
{
    float4 pos : SV_POSITION;
	float4 col : COLOR;
};

[maxvertexcount(6)]
void main( triangle GeometryInput input[3], inout LineStream<GeometryOutput> lineStream )
{
    GeometryOutput output;

	for (uint i = 0; i < 3; i++) {
	
		float4 P = mul(mul(input[i].pos, invProjection), invView);

		float n1 = input[i].nor.x;
		float n2 = input[i].nor.y;
		float n3 = input[i].nor.z;

		float4 N = float4(P.xyz + float3(n1, n2, n3) * 0.5f, 1);

		output.pos = input[i].pos;
		output.col = float4(0, 1, 0, 1);
		lineStream.Append(output);

		output.pos = mul(mul(N, view), projection);
		output.col = float4(0, 1, 0, 1);
		lineStream.Append(output);

		lineStream.RestartStrip();

		/*

		float t1 = input[i].tan.x;
		float t2 = input[i].tan.y;
		float t3 = input[i].tan.z;

		float4 T = float4(P.xyz + float3(t1, t2, t3) * 0.5f, 1);

		output.pos = input[i].pos;
		output.col = float4(1, 0, 0, 1);
		lineStream.Append(output);

		output.pos = mul(mul(T, view), projection);
		output.col = float4(1, 0, 0, 1);
		lineStream.Append(output);

		lineStream.RestartStrip();



		float b1 = input[i].bitan.x;
		float b2 = input[i].bitan.y;
		float b3 = input[i].bitan.z;

		float4 B = float4(P.xyz + float3(b1, b2, b3) * 0.5f, 1);

		output.pos = input[i].pos;
		output.col = float4(0, 0, 1, 1);
		lineStream.Append(output);

		output.pos = mul(mul(B, view), projection);
		output.col = float4(0, 0, 1, 1);
		lineStream.Append(output);

		lineStream.RestartStrip();
		
		*/
	}

	lineStream.RestartStrip();
}