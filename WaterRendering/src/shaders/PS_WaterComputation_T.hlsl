
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> prevState : register(t0);
Texture2D<float4> terrainMap : register(t1);
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
	//return float2((pos.x - 0.5f) / sizeX, (pos.y - 0.5f) / sizeY);
	return (pos.x % 2 != 0 && pos.y % 2 != 0) ? float2((pos.x - 0.5f) / sizeX, (pos.y - 0.5f) / sizeY) : float2(pos.x / sizeX, pos.y / sizeY);
}

float getTerrainHeight(float posX, float posY)
{
	float2 normPos = getNormXY(float2(posX, posY));
	return terrainMap.Sample(samplerState, float2(normPos.x, normPos.y)).r * elevationFactor;
}

float4 getPreviousState(float posX, float posY)
{
	return prevState.Load(int3(posX, posY, 0));
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

// input: position of adjacent pixel; current height, not position
float contribution(float2 adjacentPos, float hCurr) {

	float hAdj = getPreviousState(adjacentPos.x, adjacentPos.y).g;
	if(hAdj < 0) hAdj = 0;

	float terrAdj = getTerrainHeight(adjacentPos.x, adjacentPos.y);
	float terrCurr = terrainHeight;

	// determine where the adjacent pixel is due to current pixel
	// 		  [top]
	//			1
	// [left] 4-|-2 [right]
	//			3
	//		 [bottom]

	float delta_posX = posXY.x - adjacentPos.x;
	float delta_posY = posXY.y - adjacentPos.y;

	int relativePos = 0;
	float totalOutflow = 0;

	if(delta_posX == 0 && delta_posY == 1) relativePos = 1;
	if(delta_posX == -1 && delta_posY == 0) relativePos = 2;
	if(delta_posX == 0 && delta_posY == -1) relativePos = 3;
	if(delta_posX == 1 && delta_posY == 0) relativePos = 4;

	float adjTop = 0;
	float adjRight = 0;
	float adjBottom = 0;
	float adjLeft = 0;

	float terrTop = 0;
	float terrRight = 0;
	float terrBottom = 0;
	float terrLeft = 0;

	float delta_adjTop = 0;
	float delta_adjBottom = 0;
	float delta_adjRight = 0;
	float delta_adjLeft = 0;

	// if adjacent column is less or equal to current column
	// count how much to drain away from current column = negative contribution
	if(hAdj < hCurr) {

		// [[[ NEGATIVE CONTRIBUTION ]]]

		// find other less adjacent columns

		if(posXY.y - 1 >= 0)			adjTop = getPreviousState(posXY.x, posXY.y - 1).g;
		if(posXY.x + 1 <= sizeX - 1)	adjRight = getPreviousState(posXY.x + 1, posXY.y).g;
		if(posXY.y + 1 <= sizeY - 1)	adjBottom = getPreviousState(posXY.x, posXY.y + 1).g;
		if(posXY.x - 1 >= 0)			adjLeft = getPreviousState(posXY.x - 1, posXY.y).g;

		if(posXY.y - 1 >= 0)			terrTop = getTerrainHeight(posXY.x, posXY.y - 1);
		if(posXY.x + 1 <= sizeX - 1)	terrRight = getTerrainHeight(posXY.x + 1, posXY.y);;
		if(posXY.y + 1 <= sizeY - 1)	terrBottom = getTerrainHeight(posXY.x, posXY.y + 1);;
		if(posXY.x - 1 >= 0)			terrLeft = getTerrainHeight(posXY.x - 1, posXY.y);;

		// count how much should drain away
		// boundaries test

		if(posXY.y - 1 >= 0) 			delta_adjTop = (hCurr + terrCurr) - (adjTop + terrTop);
		if(posXY.x + 1 <= sizeX - 1)	delta_adjRight = (hCurr + terrCurr) - (adjRight + terrRight);
		if(posXY.y + 1 <= sizeY - 1) 	delta_adjBottom = (hCurr + terrCurr) - (adjBottom + terrBottom);
		if(posXY.x - 1 >= 0) 			delta_adjLeft = (hCurr + terrCurr) - (adjLeft + terrLeft);

		if(delta_adjTop > 0) totalOutflow += delta_adjTop;
		if(delta_adjRight > 0) totalOutflow += delta_adjRight;
		if(delta_adjBottom > 0) totalOutflow += delta_adjBottom;
		if(delta_adjLeft > 0) totalOutflow += delta_adjLeft;

		// if total outflow number is less or equal to height of adjacent column
		// it can be drained away without any calculations
		
		if(totalOutflow <= hCurr) {

			switch(relativePos)
			{
				case 1: return 0 - delta_adjTop;
				case 2: return 0 - delta_adjRight;
				case 3: return 0 - delta_adjBottom;
				case 4: return 0 - delta_adjLeft;
				default: return 0;
			}

		} else {

		// if the number is grater, additional calculations are needed

			switch(relativePos)
			{
				case 1: return 0 - (hCurr * delta_adjTop / totalOutflow);
				case 2: return 0 - (hCurr * delta_adjRight / totalOutflow);
				case 3: return 0 - (hCurr * delta_adjBottom / totalOutflow);
				case 4: return 0 - (hCurr * delta_adjLeft / totalOutflow);
				default: return 0;
			}

		}
	} else if(hAdj > hCurr) {

		// [[[ POSITIVE CONTRIBUTION ]]]

		// look at all neighbours

		if(adjacentPos.y - 1 >= 0)			adjTop = getPreviousState(adjacentPos.x, adjacentPos.y - 1).g;
		if(adjacentPos.x + 1 <= sizeX - 1)	adjRight = getPreviousState(adjacentPos.x + 1, adjacentPos.y).g;
		if(adjacentPos.y + 1 <= sizeY - 1) 	adjBottom = getPreviousState(adjacentPos.x, adjacentPos.y + 1).g;
		if(adjacentPos.x - 1 >= 0) 			adjLeft = getPreviousState(adjacentPos.x - 1, adjacentPos.y).g;

		if(adjacentPos.y - 1 >= 0)			terrTop = getTerrainHeight(adjacentPos.x, adjacentPos.y - 1);
		if(adjacentPos.x + 1 <= sizeX - 1)  terrRight = getTerrainHeight(adjacentPos.x + 1, adjacentPos.y);;
		if(adjacentPos.y + 1 <= sizeY - 1)  terrBottom = getTerrainHeight(adjacentPos.x, adjacentPos.y + 1);;
		if(adjacentPos.x - 1 >= 0) 			terrLeft = getTerrainHeight(adjacentPos.x - 1, adjacentPos.y);;

		// count how much should drain away
		// boundaries test

		if(adjacentPos.y - 1 >= 0) 			delta_adjTop = (hAdj + terrAdj) - (adjTop + terrTop);
		if(adjacentPos.x + 1 <= sizeX - 1)	delta_adjRight = (hAdj + terrAdj) - (adjRight + terrRight);
		if(adjacentPos.y + 1 <= sizeY - 1) 	delta_adjBottom = (hAdj + terrAdj) - (adjBottom + terrBottom);
		if(adjacentPos.x - 1 >= 0) 			delta_adjLeft = (hAdj + terrAdj) - (adjLeft + terrLeft);

		// if it is a positive number it means that it drains away

		if(delta_adjTop > 0) totalOutflow += delta_adjTop;
		if(delta_adjBottom > 0) totalOutflow += delta_adjBottom;
		if(delta_adjRight > 0) totalOutflow += delta_adjRight;
		if(delta_adjLeft > 0) totalOutflow += delta_adjLeft;

		// if total outflow number is less or equal to height of adjacent column
		// it can be drained away without any calculations

		// it returns oposite pixel location !!!
		
		if(totalOutflow <= hAdj) {
	
			switch(relativePos)
			{
				case 1: return delta_adjBottom;
				case 2: return delta_adjLeft;
				case 3: return delta_adjTop;
				case 4: return delta_adjRight;
				default: return 0;
			}

		} else {

		// if the number is greater, additional calculations are needed
			
			switch(relativePos)
			{
				case 1: return hAdj * delta_adjBottom / totalOutflow;
				case 2: return hAdj * delta_adjLeft / totalOutflow;
				case 3: return hAdj * delta_adjTop / totalOutflow;
				case 4: return hAdj * delta_adjRight / totalOutflow;
				default: return 0;
			}

		}
	}

	return 0;	
}

// R - momentum
// G - height
float4 compute()
{
	// position without offset
	float posX = posXY.x;
	float posY = posXY.y;

	// previous state for current pixel
	float4 prevValues = getPreviousState(posX, posY);
	
	// previous momentum
	float prevMomentum = prevValues.r;
	// previous height
	float prevHeight = prevValues.g;

	float diff = contribution(float2(posX, posY - 1), prevHeight)
				+contribution(float2(posX + 1, posY), prevHeight)
				+contribution(float2(posX, posY + 1), prevHeight)
				+contribution(float2(posX - 1, posY), prevHeight);

	float currMomentum = 0.0f;

	currMomentum = viscosity * prevMomentum + diff / 4;
	float currHeight = diff / 4 + prevHeight;

	float prevDiff = 0;
	
	/*if (currHeight < 0) {
		prevDiff = currHeight;
		currHeight = 0;
		currMomentum = 0;
	}*/

	return float4(currMomentum, currHeight, prevDiff, prevValues.a);
}

float4 copyTexture()
{
	// resource: computeTexture_0
	return prevState.Load(int3(posXY.x, posXY.y, 0)).rgba;
}

// SHADER ENTRY POINT
float4 main(PixelInput input) : SV_TARGET
{

	// current pixel coords in texel coord. system (0-1)
	normXY = getNormXY(float2(input.pos.x, input.pos.y));
	posXY = float2(input.pos.x - 0.5, input.pos.y - 0.5);

	// two actions in one frame
	// first - compute new values from a previous state (if reset or water drop is not active)
	// second - create copy of new values to a second texture which is set as a resource in the first action
	if (action == 0) {

		terrainHeight = getTerrainHeight(posXY.x, posXY.y);

		// reset to calm surface (first frame and on backspace key press)
		if (reset == 1) {
			return float4(0, 0, 0, 1); // r - momentum, g - height
		}

		// generate water drop (on enter key press)
		if (waterDrop == 1){
			
			// point where to start water drop
			float originX = normXY.x - 0.5;
			float originY = normXY.y - 0.6;

			// water drop in the middle of the grid
			float distance = getDistance(float2(originX, originY));
			float dropStrength = height;

			float4 prev = getPreviousState(posXY.x, posXY.y);

			if (distance < dropStrength) {
				float finalHeight = pow(dropStrength, 1) - pow(distance, 1);
				return float4(prev.r, prev.g + sqrt(finalHeight), prev.b, prev.a);
			}

			// water drop is not affecting this pixel
			return prev;
	}

		// compute new values depending on a previous state
		return compute();
		//return prevState.Load(int3(input.pos.x - 0.5f, input.pos.y - 0.5f, 0));

	} else {

		// just copy a texture as it is to another texture
		// it's a simply backup to have a previous state in a next frame
		return copyTexture();
	}
}