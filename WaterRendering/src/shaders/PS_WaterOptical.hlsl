
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

#include "fragment/ILight.psi"

TextureCube textureSkybox : register (t0);
Texture2D<float4> textureTerrainPersp : register (t1);
Texture2D<float4> terrainTexture : register(t2);
SamplerState samplerLinearWrap : register (s0);
SamplerState samplerLinearClamp : register (s1);

struct PixelInput
{
	float4 pos : SV_POSITION;
	float3 reflection : TEXCOORD0;
	float2 refraction : TEXCOORD1;
	float3 refractDir : TEXCOORD2;
	float3 viewDirection : POSITION0;
	float3 surfaceNormal : NORMAL;
	float3 surfacesPos : POSITION1;
};

static float h0 = 0.0055;

float4 main(PixelInput input) : SV_TARGET
{
	clip(input.surfacesPos.y - 0.00003 <= input.surfacesPos.x ? -1 : 1);

	float4 reflectiveColor = float4(textureSkybox.Sample(samplerLinearWrap, input.reflection).rgb, 1);
	float4 refractiveColor = textureTerrainPersp.Sample(samplerLinearClamp, input.refraction);

	if (refractiveColor.r == 0 && refractiveColor.g == 0 && refractiveColor.b == 0)
		refractiveColor = float4(textureSkybox.Sample(samplerLinearWrap, -input.reflection).rgb, 1);

	float fresnel = dot(normalize(input.viewDirection), input.surfaceNormal);
	float4 fresnelColor = lerp(reflectiveColor, refractiveColor, fresnel);

		//fresnelColor = reflectiveColor;

	float h = input.surfacesPos.y - input.surfacesPos.x;

	fresnelColor = lerp(refractiveColor, fresnelColor, 0.5);
	fresnelColor = lerp(float4(0.0, 0.025, 0.01, 1), fresnelColor, 0.65);

	if (h < h0) {
		fresnelColor = lerp(refractiveColor, fresnelColor, h / h0);
	}

	return fresnelColor;
}