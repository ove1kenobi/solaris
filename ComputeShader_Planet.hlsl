#include "SNoise.hlsli"
cbuffer CenterBuffer {
	float3 center;
};

struct WorldPosition {
	float x;
	float y;
	float z;
	float w; //Trash value
};

StructuredBuffer<WorldPosition> BufferIn : register(t0);
RWStructuredBuffer<WorldPosition> BufferOut : register(u0);

[numthreads(2, 2, 1)]
void cs_main( uint3 DTid : SV_DispatchThreadID )
{
	float3 dir = float3(BufferIn[DTid.x].x, BufferIn[DTid.x].y, BufferIn[DTid.x].z) - center;
	
	float changeShape = fractalNoise(float3(BufferIn[DTid.x].x / 1000, BufferIn[DTid.x].y / 1000, BufferIn[DTid.x].z / 1000));
	changeShape = (changeShape * 0.05) + 1; //So that it is between 0.5 and 1.5.
	
	dir *= changeShape;

	float changeDetail = fractalNoise(float3(dir.x, dir.y, dir.z));
	changeDetail = (changeDetail * 0.01) + 1;
	dir *= changeDetail;


	float changeRidge = fractalNoise(float3(dir.x / 1000, dir.y / 1000, dir.z / 1000));
	changeRidge = 1 - abs(changeRidge);

	dir += changeRidge * 40;


	BufferOut[DTid.x].x = dir.x;
	BufferOut[DTid.x].y = dir.y;
	BufferOut[DTid.x].z = dir.z;
	BufferOut[DTid.x].w = 0;
}