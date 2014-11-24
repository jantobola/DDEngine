
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

cbuffer Color : register(b10)
{
	float4 color;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
};

float4 main(PixelInput input) : SV_TARGET
{
	return color;
}