
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

TextureCube textureSkybox : register ( t0 );
Texture2D<float4> textureBottom : register ( t1 );
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
	float3 reflection : TEXCOORD0;
	float2 refraction : TEXCOORD1;
	float3 viewDirection : POSITION;
};

float4 main(PixelInput input) : SV_TARGET
{
	float4 reflectiveColor = float4(textureSkybox.Sample(samplerState, input.reflection).rgb, 1);
	float4 refractiveColor = textureBottom.Sample(samplerState, input.refraction);

	float3 normalVec = float3(0, 1, 0);
	float fresnel = dot(normalize(input.viewDirection), normalVec);

	float4 fresnelColor = lerp(reflectiveColor, refractiveColor, fresnel);

	float4 dullColor = float4(0.3f, 0.3f, 0.5f, 1.0f);
	return lerp(fresnelColor, dullColor, 0.1f);
}