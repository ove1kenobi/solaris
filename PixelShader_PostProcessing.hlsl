#include "GPUmath.hlsli"
Texture2DMS<float4> colorTexture	: register(t0);
Texture2DMS<float4> waterBoolTexture	: register(t1);
Texture2DMS<float4> lengthCenterTexture : register(t2);
Texture2DMS<float4> wPosTexture : register(t3);

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

cbuffer ScreenData : register(b2)
{
	float screenWidth;
	float screenHeight;
}

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

	//Check if there is supposed to be water on the point we are looking at.
	//(Might be that we are looking through water on the "side", so this info is not sufficient.
	float4 waterBool = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int j = 0; j < 4; j++) {
		waterBool += waterBoolTexture.Load(psIn.outPositionPS.xy, j);
	}
	waterBool /= 4;
	
	//Get the distance to the planets center in this pixel.
	float4 distFromCenter = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int l = 0; l < 4; l++) {
		distFromCenter += lengthCenterTexture.Load(psIn.outPositionPS.xy, l);
	}
	distFromCenter /= 4;

	//Get the world position of the pixel we are looking at.
	float4 wPos = 0.0f;
	for (int m = 0; m < 4; m++) {
		wPos += wPosTexture.Load(psIn.outPositionPS.xy, m);
	}
	wPos /= 4;

	//Calculate the depth to the point we are looking at.
	float depth = length(wPos.xyz - cameraPos.xyz);

	float4 pixelViewSpace;
	pixelViewSpace.x = (((2.0f * psIn.outPositionPS.x) / screenWidth) - 1.0f) / PMatrix[0][0];
	pixelViewSpace.y = (((-2.0f * psIn.outPositionPS.y) / screenHeight) + 1.0f) / PMatrix[1][1];
	pixelViewSpace.z = 1.0f;
	pixelViewSpace.w = 0.0f;

	//Get the direction we are looking in this pixel.
	float4 DirectionWorldSpace = mul(pixelViewSpace, inverseVMatrix);
	
	float2 closestPlanet = float2(3.402823466e+38F, -1.0f);
	float2 tempPlanet;
	float radian;
	//Go through all the planets.
	for (int k = 0; k < 50; k++) {
		//Breaks the loop when there are no more planets in the array.
		if (center[k].w == 0.0f) {
			break;
		}
		//Check intersections and put distance to near in tempPlanet.x, and the amount of water we are looking though in tempPlanet.y.
		//radius is +5 to add some water on top of the texcolor.
		tempPlanet = raySphereIntersect(center[k].xyz, center[k].w + 5, cameraPos, normalize(float3(DirectionWorldSpace.xyz)));
		//If we hit the planet and the planet is closer than the closest planet so far.
		if (tempPlanet.x != -1 && tempPlanet.x < closestPlanet.x) {
			closestPlanet = tempPlanet;
			radian = center[k].w;
		}
		//After this for-loop we have the distance to the closest planet.
	}

	//If it didnt hit a planet.
	//If the player is more than 10000 units away from the point, just return texCol.
	//If there is not supposed to be water, simply return textureColor.
	if (closestPlanet.y == -1.0f || depth >= 10000.0f || (waterBool.x == 0.0f && depth - closestPlanet.x <= 0.0f)) {
		return texCol;
	}

	//How much water we are looking through.
	float oceanViewDepth = min(closestPlanet.y, depth - closestPlanet.x);
	//If we are not looking at the planet, but through the water.
	if (oceanViewDepth < 0.0f) {
		oceanViewDepth = closestPlanet.y;
	}

	float opticalDepth = 1 - exp(-oceanViewDepth * 0.1f);
	float alpha = (1 - exp(-oceanViewDepth * 0.1)) * clamp((((-depth) / 1000) + 10), 0, 1);
	float4 oceanCol = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.f, 0.f, 0.2f, 1.0f), opticalDepth);
	return lerp(texCol, oceanCol, alpha);
}