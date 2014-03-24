
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

TextureCube objTexture;
SamplerState objSamplerState;

struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
    return objTexture.Sample( objSamplerState, input.tex );
}