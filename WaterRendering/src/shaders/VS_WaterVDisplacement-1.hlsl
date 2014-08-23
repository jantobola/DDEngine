
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> computedTexture : register( t0 );
Texture2D<float4> terrainTexture : register( t1 );
SamplerState samplerState : register( s0 );

cbuffer Matrices : register ( b0 )
{
	matrix world 			: packoffset(c0);
	matrix view 			: packoffset(c4);
	matrix projection 		: packoffset(c8);
	matrix worldInvTrans 	: packoffset(c16);
};

cbuffer Timer : register ( b1 )
{
	float time;
};

cbuffer SurfaceProps : register ( b2 )
{
	float sizeX 					: packoffset(c0.y);
	float sizeY						: packoffset(c0.z);
	float heightOffset 				: packoffset(c1.w);
	float2 refractiveTextureScale 	: packoffset(c2.x);
};

cbuffer CameraBuffer : register ( b3 )
{
	float3 cameraPosition;
}

cbuffer TerrainProps : register ( b4 )
{
	float2 terrainSize : packoffset(c0.x);
	float elevationFactor : packoffset(c0.z);
}

struct VertexInput
{
	float2 pos : POSITION;
};

struct VertexOutput 
{
    float4 pos : SV_POSITION;
	float3 reflection : TEXCOORD0;
	float2 refraction : TEXCOORD1;
	float3 refractDir : TEXCOORD2;
	float3 viewDirection : POSITION0;
	float3 surfaceNormal : NORMAL;
	float3 surfacesPos : POSITION1;
};

// input: pos.xy - 2D pixel coord
// output: pos.rgba - color components for a given pixel from computed texture
float4 getRGBA(float2 pos)
{
	return computedTexture.SampleLevel(samplerState, float2(pos.x + (0.5 / sizeX), (1 - pos.y + (0.5 / sizeY))), 0);
}

// input: pos.xy - 2D grid coord
// output: pos.xyz - 3D coords containing height in pos.y
float3 getHeight(float2 pos)
{
	float4 heights = getRGBA(pos);
	float height = heights.r + heights.g;
	return float3(pos, height).xzy;
}

VertexOutput main( VertexInput input )
{
	VertexOutput output;

	float4 surfaceHeight = getRGBA(input.pos);

	float2 position2 = input.pos;
	float4 position4 = float4(position2.x, surfaceHeight.r + surfaceHeight.g, position2.y, 1);

	output.surfacesPos = float3(surfaceHeight.g, position4.y, heightOffset);

	float3 xVec = getHeight(position2 + float2(1.0f / sizeX, 0)) - getHeight(position2 - float2(1.0f / sizeY, 0));
	float3 zVec = getHeight(position2 + float2(0, 1.0f / sizeX)) - getHeight(position2 - float2(0, 1.0f / sizeY));

	// get cross product
	float3 normal = cross(xVec, zVec);
	
    output.pos = mul(position4, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);






	// REFLECTION COORDS
	float4 vertexPosWorld = mul(position4, world);
	output.viewDirection = cameraPosition - vertexPosWorld;

	float3 normalWorld = normalize(mul(normal, world));
	output.reflection = reflect(-normalize(output.viewDirection), normalWorld);

	output.surfaceNormal = -normalWorld;






	// REFRACTION COORDS
	float3 refractDir = output.refractDir = refract( -normalize(output.viewDirection), normalWorld, 1.333f - 1.00029f);
    
	float4 screenSpace;
	float4 worldSpace;

	worldSpace.xyz = vertexPosWorld.xyz ;//+ refractDir * 0.5*sqrt(surfaceHeight.r);
	worldSpace.w = 1.0;
	
	screenSpace = mul(worldSpace, view);
	screenSpace = mul(screenSpace, projection);

	float3 screenNormal = normalize(mul(float4(normalWorld,1), view).xyz);
	
	// divide perspective
	screenSpace.xy /= screenSpace.w;
	screenSpace.xy -= screenNormal.xy * 0.1 * max(0, min(surfaceHeight.r * 100, 1)); 
	
	// [-1,-1]->[1,1] to [0,0]->[1,1],
	screenSpace.xy = screenSpace.xy * 0.5 + 0.5;
	screenSpace.y = 1 - screenSpace.y; // for sampling
	output.refraction = screenSpace;




	// OLD WAY
    /*float dist = -position4.y / refractDir.y;
    float2 base_pos = vertexPosWorld.xz + 0*dist * refractDir.xz;
    output.refraction = base_pos; * (refractiveTextureScale);*/

	return output;
}