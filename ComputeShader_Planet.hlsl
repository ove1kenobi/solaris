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

[numthreads(512, 1, 1)]
void cs_main( uint3 DTid : SV_DispatchThreadID )
{
	float oceanDepthMultiplier = 20;
	float oceanFloorDepth = 0.9;
	float oceanFloorSmoothing = 1.0f;
	float mountainBlend = 1.2f;

	//simple noise
	/*
	int numLayers = 4;
	float lacunarity = 2;
	float persistence = 0.5f;
	float scale = 1;
	float elevation = 1;
	float verticalShift = 0;
	*/
	//Ridgid
	
	int numLayers = 5;
	float lacunarity = 2;
	float persistence = 0.5f;
	float scale = 1;
	float power = 2;
	float elevation = 1;
	float gain = 1;
	float verticalShift = 0;
	float peakSmoothing = 1.0f;
	float4 params[3];
	params[0].x = center.x;
	params[0].y = center.y;
	params[0].z = center.z;
	params[0].w = numLayers;
	params[1].x = persistence;
	params[1].y = lacunarity;
	params[1].z = scale;
	params[1].w = elevation;
	params[2].x = power;
	params[2].y = gain;
	params[2].z = verticalShift;
	params[2].w = peakSmoothing;
	float3 pos = float3(BufferIn[DTid.x].x, BufferIn[DTid.x].y, BufferIn[DTid.x].z) + center;
	float3 dir = float3(BufferIn[DTid.x].x, BufferIn[DTid.x].y, BufferIn[DTid.x].z);
	pos = pos / radius;
	float continentShape = simpleNoise(pos, params);
	continentShape = smoothMax(continentShape, -oceanFloorDepth, oceanFloorSmoothing);
	
	if (continentShape < 0) {
		continentShape *= 1 + oceanDepthMultiplier;
	}
	
	float ridgeNoise = smoothedRidgidNoise(pos, params);
	float mask = Blend(0, mountainBlend, simpleNoise(pos, params));
	
	float finalHeight = 1 + ridgeNoise * 0.1f + continentShape * 0.01f;
	
	dir *= finalHeight;
	BufferOut[DTid.x].x = dir.x;
	BufferOut[DTid.x].y = dir.y;
	BufferOut[DTid.x].z = dir.z;
	BufferOut[DTid.x].w = 0;
	
	/*
	BufferOut[DTid.x].x = BufferIn[DTid.x].x;
	BufferOut[DTid.x].y = BufferIn[DTid.x].y;
	BufferOut[DTid.x].z = BufferIn[DTid.x].z;
	BufferOut[DTid.x].w = 0;
	*/
}