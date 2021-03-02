#include "SNoise.hlsli"
cbuffer PlanetConstants {
	float3 center;
	float radius;
};

struct WorldPosition {
	float x;
	float y;
	float z;
	float w; //Trash value
};

StructuredBuffer<WorldPosition> BufferIn : register(t0);
RWStructuredBuffer<WorldPosition> BufferOut : register(u0);

[numthreads(100, 1, 1)]
void cs_main( uint3 DTid : SV_DispatchThreadID )
{
	float oceanDepthMultiplier = 50.f; //Multiplies the change in height.
	float oceanFloorDepth = 0.3f; //Deepest ocean. Percent reduction of radius (0.3 = 70% of radius).
	float oceanFloorSmoothing = 1.0f; //Smoothes everything out "from below". Higher value = less ocean.
	float mountainBlend = 10.f;

	//simple noise
	int numLayers = 4; //Number of iterations, to get more smooth noise.
	float persistence = 0.3f; //The higher the more "spottier" the water becomes, less consistent water
	float lacunarity = 4.0f; //The higher the more jagged the bottom becomes.

	//Should depend on radius.
	float scale = 1.0f; //Determines how "big of a part" of the noise "texture" the points get sampled from
	float multiplier = 2.0f; //Makes the effect more drastic.
	float verticalShift = 0.0f; //Shifts the height by a static amount. 
	
	
	float4 params[3];
	params[0].x = center.x;
	params[0].y = center.y;
	params[0].z = center.z;
	params[0].w = numLayers;
	params[1].x = persistence;
	params[1].y = lacunarity;
	params[1].z = scale;
	params[1].w = multiplier;
	params[2].z = verticalShift;

	float3 pos = float3(BufferIn[DTid.x].x, BufferIn[DTid.x].y, BufferIn[DTid.x].z) + center;
	float3 dir = float3(BufferIn[DTid.x].x, BufferIn[DTid.x].y, BufferIn[DTid.x].z);
	pos = pos / radius;
	float continentShape = simpleNoise(pos, params);
	continentShape = smoothMax(continentShape, -oceanFloorDepth, oceanFloorSmoothing);
	
	if (continentShape < 0) {
		continentShape *= 1 + oceanDepthMultiplier;
	}
	
	//Ridgid
	numLayers = 5; //Number of iterations, to get more smooth noise.
	persistence = 0.5f; //More of a drastic difference between different points.
	lacunarity = 2.0f; //More jagged mountaintops

	//Should depend on radius
	scale = 1.0f; //Determines how "big of a part" of the noise "texture" the points get sampled from
	multiplier = 1.0f; //Makes the effect more drastic. 
	float power = 2.0f; //Exponentially reduces the effect.
	float gain = 1.0f; //Icreases the amount of mountains on the surface.
	verticalShift = 0; //Shifts the height by a static amount. 
	float peakSmoothing = 1.0f; //Determines how much the mountain tops are smoothed

	params[0].w = numLayers;
	params[1].x = persistence;
	params[1].y = lacunarity;
	params[1].z = scale;
	params[1].w = multiplier;
	params[2].x = power;
	params[2].y = gain;
	params[2].z = verticalShift;
	params[2].w = peakSmoothing;

	float ridgeNoise = smoothedRidgidNoise(pos, params);
	float mask = Blend(0, mountainBlend, simpleNoise(pos, params));
	
	//Changing ridgeNoise multiplier to 0.01f or 0.05f creates more earth-like planets
	float finalHeight = 1 + ridgeNoise * 0.1f + continentShape * 0.01f;
	
	dir *= finalHeight;
	BufferOut[DTid.x].x = dir.x;
	BufferOut[DTid.x].y = dir.y;
	BufferOut[DTid.x].z = dir.z;
	BufferOut[DTid.x].w = 0;
}