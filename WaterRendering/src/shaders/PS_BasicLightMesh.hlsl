//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

#include "fragment/ILight.psi"

Texture2D<float4> texture_0 : register ( t0 );
Texture2D<float4> texture_1 : register ( t1 );

SamplerState sampler_0 : register ( s0 );

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
	float3 nor : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
	return BaseLight(texture_0, sampler_0, input.nor, input.tex);
}