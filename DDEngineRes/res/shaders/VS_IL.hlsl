float4 main(float4 position : POSITION) : SV_POSITION
{
	return position;
}

struct IL_POS_TEX
{
	float3 pos : SV_Position;
	float2 tex : TEXCOORD0;
};

float4 POS_TEX(IL_POS_TEX input) : SV_Position
{
	return float4(0, 0, 0, 0);
}

struct IL_POS_COL
{
	float3 pos : SV_Position;
	float4 col : COLOR0;
};

float4 POS_COL(IL_POS_COL input) : SV_Position
{
	return float4(0, 0, 0, 0);
}

struct IL_POS_NOR
{
	float3 pos : SV_Position;
	float3 nor : NORMAL0;
};

float4 POS_NOR(IL_POS_NOR input) : SV_Position
{
	return float4(0, 0, 0, 0);
}

struct IL_POS_NOR_COL
{
	float3 pos : SV_Position;
	float3 nor : NORMAL0;
	float4 col : COLOR0;
};

float4 POS_NOR_COL(IL_POS_NOR_COL input) : SV_Position
{
	return float4(0, 0, 0, 0);
}

struct IL_POS_COL_TEX
{
	float3 pos : SV_Position;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

float4 POS_COL_TEX(IL_POS_COL_TEX input) : SV_Position
{
	return float4(0, 0, 0, 0);
}

struct IL_POS_NOR_TEX
{
	float3 pos : SV_Position;
	float3 nor : NORMAL0;
	float2 tex : TEXCOORD0;
};

float4 POS_NOR_TEX(IL_POS_NOR_TEX input) : SV_Position
{
	return float4(0, 0, 0, 0);
}

struct IL_POS_NOR_COL_TEX
{
	float3 pos : SV_Position;
	float3 nor : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

float4 POS_NOR_COL_TEX(IL_POS_NOR_COL_TEX input) : SV_Position
{
	return float4(0, 0, 0, 0);
}

struct IL_POS_NOR_TAN_COL_TEX
{
	float3 pos : SV_Position;
	float3 nor : NORMAL0;
	float4 tan : TANGENT0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

float4 POS_NOR_TAN_COL_TEX(IL_POS_NOR_TAN_COL_TEX input) : SV_Position
{
	return float4(0, 0, 0, 0);
}