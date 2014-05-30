
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

TextureCube cubeTexture;
SamplerState samplerState;

struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
    return cubeTexture.Sample( samplerState, input.tex );
}