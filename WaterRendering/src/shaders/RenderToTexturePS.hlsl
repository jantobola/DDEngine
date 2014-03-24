
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> prevState : register(t0);
SamplerState samp : register(s0);

cbuffer WaterProps : register ( b0 )
{
	float height;
	float sizeX;
	float sizeY;
	int action;
	float viscosity;
	int waterDrop;
	int reset;
};

cbuffer Timer : register (b1)
{
	float time;
}

struct PixelInput
{
    float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

// computes a distance from origin
// arg coords - texel coords (0-1)
float getDistance(float2 coords)
{
	float distX = coords.x - 0.5f / 2; // 0.5 is an origin (middle)
	float distY = coords.y - 0.5f / 2;

	float powC = pow(distX, 2) + pow(distY, 2);
	return sqrt(powC);
}
// R - setrvacnost
// G - height
float4 compute(PixelInput input)
{	
	// position without offset
	float posX = input.pos.x - 0.5f;
	float posY = input.pos.y - 0.5f;

	// previous state for current pixel
	float4 prevValues = prevState.Load(int3(posX, posY, 0));
	
	// previous attenuation
	float prevAtten = prevValues.r;
	// previous height
	float prevHeight = prevValues.g;

	// adjacent pixels from a previous state 4-direction
	float hBottom = prevState.Load(int3(posX, posY + 1, 0)).g;
	float hRight =	prevState.Load(int3(posX + 1, posY, 0)).g;
	float hTop =	prevState.Load(int3(posX, posY - 1, 0)).g;
	float hLeft =	prevState.Load(int3(posX - 1, posY, 0)).g;

	float dhTop = 0.0f;
	float dhRight = 0.0f;
	float dhBottom = 0.0f;
	float dhLeft = 0.0f;

	// boundaries
	if (posX - 1 >= 0.0f)		dhLeft = hLeft - prevHeight;			//else return float4(1, 0, 0, 1);
	if (posX + 1 <= sizeX - 1)	dhRight = hRight - prevHeight;			//else return float4(1, 0, 0, 1);
	if (posY - 1 >= 0.0f)		dhTop = hTop - prevHeight;				//else return float4(1, 0, 0, 1);
	if (posY + 1 <= sizeY - 1)	dhBottom = hBottom - prevHeight;		//else return float4(1, 0, 0, 1);

	float diff = dhTop + dhRight + dhBottom + dhLeft;
	
	float currAtten = (viscosity - abs(diff / 8)) * prevAtten + diff / 4;
	float currHeight = currAtten + prevHeight;

	if (currHeight <= 0) currHeight = prevHeight;

	return float4(currAtten, currHeight, prevValues.b, prevValues.a);
}

float4 copyTexture(PixelInput input)
{
	// position without offset
	float posX = input.pos.x - 0.5f;
	float posY = input.pos.y - 0.5f;

	// resource: computeTexture_0
	return prevState.Load(int3(posX, posY, 0)).rgba;
}

float4 main(PixelInput input) : SV_TARGET
{
	// two actions in one frame
	// first - compute new values from a previous state (if reset or water drop is not active)
	// second - create copy of new values to a second texture which is set as a resource in the first action
	if (action == 0) {

		// reset to calm surface (first frame and on backspace key press)
		if (reset == 1) {
			return float4(0, 0, 1, 1); // r - setrvacnost, g - height
		}

		// generate water drop (on enter key press)
		if (waterDrop == 1){
			
			// water drop in the middle of the grid
			float distance = getDistance(float2((input.pos.x - 0.5f) / sizeX, (input.pos.y - 0.5f) / sizeY));
			// just a small water drop
			float dropStrength = height;

			float4 prev = prevState.Load(int3(input.pos.x - 0.5f, input.pos.y - 0.5f, 0));

			if (distance < dropStrength) {
				float finalHeight = pow(dropStrength, 1) - pow(distance, 1);
				return float4(prev.r, prev.g + sqrt(finalHeight), prev.b, prev.a);
			}
			

			//float4 prev = prevState.Load(int3(input.pos.x - 0.5f, input.pos.y - 0.5f, 0));
			//if (input.pos.x - 0.5f == sizeX / 4 && input.pos.y - 0.5f == sizeY / 2) 
			//return float4(prev.r, prev.g + height, prev.b, prev.a);

			return prev;
	}

		// compute new values depending on previous state
		return compute(input);
		//return prevState.Load(int3(input.pos.x - 0.5f, input.pos.y - 0.5f, 0));

	} else {

		// just copy a texture as it is to another texture
		// it's a simply backup to have a previous state in a next frame
		return copyTexture(input);
	}
}