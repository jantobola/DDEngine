
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

struct PixelInput
{
    float4 pos : SV_POSITION;
};

float4 main(PixelInput input) : SV_TARGET
{
	return float4(0, 1, 0, 1);
}