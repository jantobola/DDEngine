
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

Texture2D<float4> computedTexture : register( t0 );
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

struct VertexInput 
{
	float2 pos : POSITION;
};

struct VertexOutput 
{
    float4 pos : SV_POSITION;
	float3 reflection : TEXCOORD0;
	float2 refraction : TEXCOORD1;
	float3 viewDirection : POSITION;
};

// input: pos.xy - 2D pixel coord
// output: pos.rgba - color components for a given pixel from computed texture
float4 getRGBA(float2 pos)
{
	return computedTexture.Load(int3(pos.x * sizeX, (1 - pos.y) * sizeY, 0));
}

// input: pos.xy - 2D grid coord
// output: pos.xyz - 3D coords containing height in pos.y
float3 getHeight(float2 pos)
{
	float height = getRGBA(pos).g; // height is in a G component
	return float3(pos, height).xzy;
}

VertexOutput main( VertexInput input )
{
	VertexOutput output;

	float4 rgba = getRGBA(input.pos);

	float2 position2 = input.pos;
	float4 position4 = float4(position2.x, rgba.g + heightOffset, position2.y, 1);

	float3 xVec = getHeight(position2 + float2(1.0f / sizeX, 0)) - getHeight(position2 - float2(1.0f / sizeY, 0));
	float3 zVec = getHeight(position2 + float2(0, 1.0f / sizeX)) - getHeight(position2 - float2(0, 1.0f / sizeY));

	// get cross product
	float3 normal = cross(xVec, zVec);
	
    output.pos = mul(position4, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);


	// REFLECTION COORDS
	float4 vertPos = mul(position4, world);
	output.viewDirection = cameraPosition - vertPos;

	float3 norm = normalize(mul(normal, world));
	output.reflection = reflect(-normalize(output.viewDirection), norm);

	// REFRACTION COORDS
	float3 refract_dir = refract( -cameraPosition, norm, 1 / 1.333f);
    
    float dist = -position4.y / refract_dir.y;
    float2 base_pos = vertPos.xz + dist * refract_dir.xz;
    output.refraction = base_pos * refractiveTextureScale;

	return output;
}