#include "GPUmath.hlsli"
Texture2DMS<float4> colorTexture	: register(t0);
Texture2DMS<float> depthTexture	: register(t1);

cbuffer PlanetData : register(b0)
{
	float4 center[50]; //Radius is in w.
};

cbuffer CameraData : register(b1)
{
	float4 cameraDir;
	float4 cameraPos;
	matrix inverseVMatrix;
	matrix inversePMatrix;
	float4x4 PMatrix;
};

struct PS_IN
{
	float4 outPositionPS : SV_Position;
	float4 outPositionWS : POSITION;
	float2 outTexUVPS    : TEXUV;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
	//Calculate texture color from what is seen.
	float4 texCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		texCol += colorTexture.Load(psIn.outPositionPS.xy, i);
	}
	texCol /= 4;


	float depth = 0.0f;
	for (int j = 0; j < 4; j++) {
		depth += depthTexture.Load(psIn.outPositionPS.xy, j);
	}

	depth /= 4;
	
	float4 pixelViewSpace;
	pixelViewSpace.x = (((2.0f * psIn.outPositionPS.x) / 1920.0f) - 1.0f) / PMatrix[0][0];
	pixelViewSpace.y = (((-2.0f * psIn.outPositionPS.y) / 1080.0f) + 1.0f) / PMatrix[1][1];
	pixelViewSpace.z = 1.0f;
	pixelViewSpace.w = 0.0f;

	float4 DirectionWorldSpace = mul(pixelViewSpace, inverseVMatrix);

	//depth = (1.0f / (((1 - 100000 / 0.1f)) * depth + (100000 / 0.1f)));
	depth = (1.0f / (((1.0f - 100000.0f / 0.1f) / 100000.0f) * depth + ((100000.0f / 0.1f) / 100000.0f))) * length(float3(DirectionWorldSpace.xyz));
	
	//return float4(depth / 1000, depth / 1000, depth / 1000, 1.0f);
	float4 originVector = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 originWorldSpaceVector = mul(originVector, inverseVMatrix);
	
	float2 closestPlanet = float2(3.402823466e+38F, 0.0f);
	float2 tempPlanet;
	for (int k = 0; k < 50; k++) {
		//Breaks the loop when there are no more planets in the array.
		if (center[k].w == 0.0f) {
			break;
		}
		tempPlanet = raySphereIntersect(center[k].xyz, center[k].w + 5, cameraPos, normalize(float3(DirectionWorldSpace.xyz)));
		//If we hit the planet and the planet is closer than the closest planet so far.
		if (tempPlanet.x != -1 && tempPlanet.x < closestPlanet.x) {
			closestPlanet = tempPlanet;
		}
		//After this for-loop we have the distance to the closest planet.
	}
	

	float oceanViewDepth = min(closestPlanet.y, depth - closestPlanet.x);
	
	if (oceanViewDepth > 0.0f) {

		float opticalDepth = 1 - exp(-oceanViewDepth * 0.05f);
		float alpha = 1 - exp(-oceanViewDepth * 0.05);
		float4 oceanCol = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.f, 0.f, 0.2f, 1.0f), opticalDepth);
		return lerp(texCol, oceanCol, alpha);
	}

	return texCol;
	
}