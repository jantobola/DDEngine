
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

#include "fragment/ILight.psi"

TextureCube textureSkybox : register ( t0 );
Texture2D<float4> textureBottom : register ( t1 );
Texture2D<float4> terrainTexture : register( t2 );
SamplerState samplerState : register ( s0 );

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

float4 main(PixelInput input) : SV_TARGET
{

	clip(input.surfacesPos.y /*+ input.surfacesPos.z*/ <= input.surfacesPos.x ? -1 : 1);

	float4 reflectiveColor = float4(textureSkybox.Sample(samplerState, input.reflection).rgb, 1);

	// here sample from refraction map instead of textureBoorom with BaseLight function
	float4 refractiveColor = BaseLight(textureBottom, samplerState, -input.surfaceNormal, input.refraction);

	refractiveColor = float4(0.2, 0.2, 0.8, 1);

	//float3 normalVec = float3(0, 1, 0);
	float fresnel = dot(normalize(input.viewDirection), input.surfaceNormal);
	float4 fresnelColor = lerp(reflectiveColor, refractiveColor, fresnel);

	return fresnelColor;
}