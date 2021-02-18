#include "GPUmath.hlsli"
Texture2DMS<float4> colorTexture	: register(t0);
Texture2DMS<float> depthTexture	: register(t1);

cbuffer PlanetData
{
	float4 center[50]; //Radius is in w.
};

cbuffer CameraData
{
	float4 cameraDir;
	float4 cameraPos;
	matrix inverseVMatrix;
	matrix inversePMatrix;
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
	for (int i = 0; i < 4; i++) {
		depth += depthTexture.Load(psIn.outPositionPS.xy, i);
	}

	depth /= 4;
	depth = (1.0f / (((1 - 100000 / 0.1f)) * depth + (100000 / 0.1f)));
	depth = (1.0f / (((1 - 100000 / 0.1f) / 100000) * depth + ((100000 / 0.1f) / 100000))) * length(float3(cameraDir.xyz));
	//(1-far/near) / far
	//(far/near) / far
	//Calculate the normalized view direction
	float3 rayDir = normalize(float3(cameraDir.xyz));

	//Calculate the current pixels worldposition.
	psIn.outPositionWS.x *= (1920.f / 2.f);
	psIn.outPositionWS.y *= (1080.f / 2.f);
	psIn.outPositionWS.z = 0.1f;
	float4 pixelPos = mul(psIn.outPositionWS, inversePMatrix);
	pixelPos = mul(pixelPos, inverseVMatrix);
	pixelPos /= pixelPos.w;
	
	return pixelPos;
	//rayDir = normalize(pixelPos.xyz - cameraPos.xyz);

	//pixelPos.z = depth - cameraPos.z;

	float2 closestPlanet = float2(3.402823466e+38F, 0);
	float2 tempPlanet;
	for (int i = 0; i < 50; i++) {
		//Breaks the loop when there are no more planets in the array.
		if (center[i].w == 0.0f) {
			break;
		}
		//								ARG3 NEEDS TO BE THE ORIGIN! I.E THE PIXEL IN THE SRV's POSITION BUT IN WORLDSPACE
		//								ARG4 NEEDS TO BE THE DIRECTION FROM ORIGIN TO THE WORLDPOS IT IS LOOKING AT! (get by transforming pixel in SRV to world space but also take depth into account?)
		tempPlanet = raySphereIntersect(center[i].xyz, center[i].w + 500, float3(pixelPos.xyz), normalize(float3(cameraDir.xyz)));
		//If we hit the planet and the planet is closer than the closest planet so far.
		if (tempPlanet.x != -1 && tempPlanet.x < closestPlanet.x) {
			closestPlanet = tempPlanet;
		}
		//After this for-loop we have the distance to the closest planet.
	}
	
	float oceanViewDepth = min(closestPlanet.y, depth - closestPlanet.x);
	if (oceanViewDepth > 0) {
		float opticalDepth = 1 - exp(-oceanViewDepth * 10);
		float alpha = 1 - exp(-oceanViewDepth * 2);
		float4 oceanCol = lerp(float4(0.f, 0.f, 0.f, 1.f), float4(0.f, 0.f, 1.f, 1.f), opticalDepth);
		return lerp(texCol, oceanCol, alpha);
	}

	return texCol;
}