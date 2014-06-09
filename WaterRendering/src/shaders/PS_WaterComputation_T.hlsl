
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> momentumTex : register(t0);
Texture2D<float4> heightTex : register(t1);
Texture2D<float4> terrainTex : register(t2);
Texture2D<float4> copyTex : register(t3);

SamplerState samplerState : register(s0);

static float terrainHeight;
static float2 normXY;
static float2 posXY;

cbuffer WaterProps : register ( b0 )
{
	float height;
	float sizeX;
	float sizeY;
	int action;

	float viscosity;
	int waterDrop;
	int reset;
	float heightOffset;
};

cbuffer Timer : register (b1)
{
	float time;		// in miliseconds
	float timeCycle;	// sum of delta times, reset when timeStep overlaped
	float timeStep;
}

cbuffer TerrainProps : register (b2)
{
	float elevationFactor : packoffset(c0.z);
}

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

float2 getNormXY(float2 pos)
{
	int2 ipos;
	float fx = modf(pos.x, ipos.x);
	float fy = modf(pos.y, ipos.y);

	return (fx == 0 && fy == 0) ? float2((pos.x + 0.5f) / sizeX, (pos.y + 0.5f) / sizeY) : float2(pos.x / sizeX, pos.y / sizeY);
}

float getTerrainHeight(float posX, float posY)
{
	float2 normPos = getNormXY(float2(posX, posY));
	return terrainTex.Sample(samplerState, float2(normPos.x, normPos.y)).r * elevationFactor;
}

float getTerrainHeight(float2 tex)
{
	return terrainTex.Sample(samplerState, float2(tex.x, tex.y)).r * elevationFactor;
}

float4 getPrevMomentum(float posX, float posY)
{
	return momentumTex.Load(int3(posX, posY, 0));
}

float4 getPrevHeight(float posX, float posY)
{
	return heightTex.Load(int3(posX, posY, 0));
}

float4 copyTexture()
{
	return copyTex.Load(int3(posXY.x, posXY.y, 0)).rgba;
}

// compute a distance from an origin
// arg coords - texel coords (0-1)
float getDistance(float2 coords)
{
	float distX = coords.x; // 0.5 is an origin (middle)
	float distY = coords.y;

	float powC = pow(distX, 2) + pow(distY, 2);
	return sqrt(powC);
}

float4 invokeWaterDrop() 
{
	// point where to start water drop
	float originX = normXY.x - 0.75;
	float originY = normXY.y - 0.99;

	// water drop in the middle of the grid
	float distance = getDistance(float2(originX, originY));
	float dropStrength = height;

	float4 prev = getPrevHeight(posXY.x, posXY.y);

	if (distance < dropStrength) {
		float finalHeight = pow(dropStrength, 1) - pow(distance, 1);
		return float4(prev.r + sqrt(finalHeight), prev.g, prev.b, prev.a);
	}

	// water drop is not affecting this pixel
	return prev;
}

float4 invokeReset() 
{
	return float4(0, 0, 0, 0);
}

void getPositions(PixelInput input) 
{
	// current pixel coords in texel coord. system (0-1)
	normXY = getNormXY(float2(input.pos.x, input.pos.y));
	posXY = float2(input.pos.x - 0.5, input.pos.y - 0.5);
}

float sum(float4 c)
{
	return c.x + c.y + c.z + c.w;
}

float mdiv(float x, float y, float z)
{
	return x * y / z;
}

float con(float2 aPos, float pm, float ph)
{
	float pah = getPrevHeight(aPos.x, aPos.y);
	float pat = getTerrainHeight(aPos.x, aPos.y);
	//float outflow = max((ph + terrainHeight) - (pah + pat), 0);
	float outflow = (ph + terrainHeight) - (pah + pat); 

	if(aPos.x < 0 || aPos.y < 0 || aPos.x > sizeX - 1 || aPos.y > sizeY - 1) outflow = 0;
	//return viscosity * pm + outflow; // TIP: mad intrinsic function in shader model 5
	return max(viscosity * pm + outflow, 0);
}

// RGBA - top, right, bottom, left
float4 calcMomentum()
{
	float4 pm = getPrevMomentum(posXY.x, posXY.y);
	float ph = getPrevHeight(posXY.x, posXY.y).r;

	float mT = con(float2(posXY.x, posXY.y - 1), pm.x, ph);
	float mR = con(float2(posXY.x + 1, posXY.y), pm.y, ph);
	float mB = con(float2(posXY.x, posXY.y + 1), pm.z, ph);
	float mL = con(float2(posXY.x - 1, posXY.y), pm.w, ph);

	float sumM = sum(float4(mT, mR, mB, mL));

	if(ph < sumM) {
		mT = mdiv(ph, mT, sumM);
		mR = mdiv(ph, mR, sumM);
		mB = mdiv(ph, mB, sumM);
		mL = mdiv(ph, mL, sumM);
	}

	return float4(mT, mR, mB, mL);
}

// R - height
float4 calcHeight() 
{
	float4 pm = getPrevMomentum(posXY.x, posXY.y);
	float ph = getPrevHeight(posXY.x, posXY.y).r;

	float sumPM = sum(pm);

	float4 pmT = getPrevMomentum(posXY.x, posXY.y - 1);
	float4 pmR = getPrevMomentum(posXY.x + 1, posXY.y);
	float4 pmB = getPrevMomentum(posXY.x, posXY.y + 1);
	float4 pmL = getPrevMomentum(posXY.x - 1, posXY.y);

	float sumAPM = sum(float4(pmT.z, pmR.w, pmB.x, pmL.y));
	float ch = ph - sumPM / 4 + sumAPM / 4;

	return float4(ch + (invokeWaterDrop().r - ph), terrainHeight, 0, 0);
}

// SHADER ENTRY POINT
float4 main(PixelInput input) : SV_TARGET
{

	getPositions(input);
	terrainHeight = getTerrainHeight(posXY.x, posXY.y);

	switch(action) {

		case 0:

			// reset to calm surface (first frame and on backspace key press)
			if (reset == 1) return invokeReset();
			return calcMomentum();

		case 1:

			// reset to calm surface (first frame and on backspace key press)
			if (reset == 1) return invokeReset();
			// generate water drop (on enter key press)
			if (waterDrop == 1) return invokeWaterDrop();

			return calcHeight();

		case 2:

			return copyTexture();

		default:

			return invokeReset();

	}

}