
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

struct PixelInput
{
    float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4 main(PixelInput input) : SV_TARGET
{
	return input.col;
}