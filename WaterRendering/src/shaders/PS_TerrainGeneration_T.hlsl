
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

#define HEIGHT 0.1f
#define TROUGH_FACTOR 0
#define TILT_FACTOR 0.5

// SHADER ENTRY POINT
float4 main(PixelInput input) : SV_TARGET
{
	float dust = 1;
	float z = HEIGHT;
	float height = z;
	float y = 0.5f - input.tex.y;

	height = pow(y * TROUGH_FACTOR, 2);
	if(height > z) {
		height = z;
		dust = 0;
	}
	
	height += input.tex.x * TILT_FACTOR;
	
	return float4(height, height, dust, 1);
}