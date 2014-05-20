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

	float3 normal = normalize(-input.nor);

	float4 rock = texture_0.Sample(sampler_0, input.tex);
	float4 grass = texture_1.Sample(sampler_0, input.tex);

	float slope = 1 - normal.y;
	float blend = 0;
	float4 final = (1, 1, 1, 1);

	float slopeFactor = 0.3;

	if(slope < slopeFactor) {
		blend = slope / slopeFactor;
        final = lerp(grass, rock, blend);
    }

	if(slope >= slopeFactor) {
        //blend = (slope - slopeFactor) / (1 - slopeFactor);
        final = rock;
    }

	return BaseLight(final, normal, input.tex);
}