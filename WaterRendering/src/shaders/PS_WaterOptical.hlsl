
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> textureObject : register ( t0 );
SamplerState samplerState : register ( s0 );

cbuffer LightProps : register (b5)
{
	float3 direction;
	float4 ambient;
	float4 diffuse;
	matrix identity;
}

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
	float3 nor : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
	input.nor = normalize(input.nor);

	float4 ambient = float4(0.0, 0.0, 0.1, 1);
	float4 diffuse = float4(0.0, 0.0, 0.8, 1);

	float4 diff = textureObject.Sample(samplerState, input.tex);

	float3 finalColor;
	finalColor = diff * ambient;
	finalColor += saturate(dot(direction, input.nor) * diff * diffuse);

	return float4(finalColor - 0.4, 0.2f);
}