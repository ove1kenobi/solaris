#include "GPUmath.hlsli"
Texture2DMS<float4> colorTexture		: register(t0);
Texture2DMS<float4> wPosTexture			: register(t1); //LengthToCenter in w.
Texture2DMS<float4> normalTexture		: register(t2);	//Check this w to see if skybox is hit. (w = 0.5 if skybox is hit.)
Texture2DMS<float4> waterSphereTexture	: register(t3);
TextureCube shadowMap					: register(t4);
SamplerState pointSampler				: register(s0);

cbuffer PlanetData : register(b0)
{
	float4 center[50]; //Radius is in w.
	float4 sun; //Radius is in w.
};

cbuffer CameraData : register(b1)
{
	float4 cameraPos;
	float4x4 inverseVMatrix;
	float4x4 PMatrix;
};

cbuffer ScreenData : register(b2)
{
	float screenWidth;
	float screenHeight;
}

cbuffer lightConstantBuffer : register(b3)
{
	float3 ambientColor;
	float padding1;
	float3 diffuseColor;
	float padding2;
	float3 lightPositionWS;
	float diffuseLightIntensity;
	float3 padding3;
	float ambientLightIntensity;
	float2 padding4;
	float specularPower;
	float specularIntensity;
}

cbuffer shadowData : register(b4)
{
    float shadowBias;
    float3 shadowPadding;
}

struct PS_IN
{
	float4 outPositionPS : SV_Position;
};

static const float farPlane = 100000.0f;

float CalculateShadowFactor(in float3 positionWS)
{
    /*Create the vector from the surface to the light*/
    float3 surfaceToLight = positionWS - lightPositionWS;
    /*This vector can act as the direction to sample from the cubemap (no need to normalize! =) )*/
    float closestTexelDepth = shadowMap.Sample(pointSampler, surfaceToLight).r;
    /*The value sampled is in the range 0-1, expand to full range*/
	closestTexelDepth *= farPlane;
    /*Get the current depth of the surface as seen from the light being used now in PS*/
    float currentSurfaceDepth = length(surfaceToLight);
    /*Is the current depth as seen from the light (minus bias) greater than the depth saved in the texture?
      If so, this specific surface is in shadow, else it's not!*/
    float shadow = currentSurfaceDepth - shadowBias > closestTexelDepth ? 1.0f : 0.0f;
    return shadow;
}

float4 ps_main(in PS_IN psIn) : SV_TARGET
{	
	//Calculate texture color from what is seen.
	float4 texCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		texCol += colorTexture.Load(psIn.outPositionPS.xy, i);
	}
	texCol *= 0.25f;

	//No multisampling
	float4 wPos = wPosTexture.Load(psIn.outPositionPS.xy, 0);
	
	//No multisampling
	float4 waterSphere = waterSphereTexture.Load(psIn.outPositionPS.xy, 0);
	
	//If it does not hit water or a planet / ship / asteroid.
	if (wPos.x == 0.5f && waterSphere.x == 0.5f) {
		return texCol;
	}
	
	//Get the distance to the planets center in this pixel.
	float distFromCenter = wPos.w;

	wPos.w = 1.0f; //wPos is a point.

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
	float3 centerPlanet;
	//Go through all the planets.
	for (int k = 0; k < 50; k++) {
		//Breaks the loop when there are no more planets in the array.
		if (center[k].w == 0.0f) {
			break;
		}
		//Check intersections and put distance to near in tempPlanet.x, and the amount of water we are looking though in tempPlanet.y.
		//radius is +5 to add some water on more than just the water colored triangles.
        tempPlanet = raySphereIntersect(center[k].xyz, center[k].w + (center[k].w * 0.03f), cameraPos, normalize(float3(DirectionWorldSpace.xyz)));
		//If we hit the planet and the planet is closer than the closest planet so far.
		if (tempPlanet.x != -1 && tempPlanet.x < closestPlanet.x) {
			closestPlanet = tempPlanet;
			radian = center[k].w;
			centerPlanet = center[k].xyz;
		}
		//After this for-loop we have the distance to the closest planet.
	}

	// Get the normal in the point we are looking at.
	float4 normalTemp = normalTexture.Load(psIn.outPositionPS.xy, 0);
	 //Check w to see if we hit skybox (is 0.5 if we hit skybox)
	float3 normal = normalize(normalTemp.xyz);

	//Skybox & sun
	tempPlanet = raySphereIntersect(sun.xyz, sun.w, cameraPos, normalize(float3(DirectionWorldSpace.xyz)));
	if ((closestPlanet.y == -1.0f && normalTemp.w == 0.5f) || (tempPlanet.x != -1.0f && depth > tempPlanet.x && tempPlanet.x < closestPlanet.x)) {
		return texCol;
	}
	
	//Shadows:
    float shadow = CalculateShadowFactor(wPos.xyz);

	//If it hit a planet.
	//And the player is less than 12000 units away from the point.
	//And if there is supposed to be water in that pixel or we are looking through water.
	//Calculate water
	[flatten]
	if ((closestPlanet.y != -1.0f && depth < 12000.0f && (depth - closestPlanet.x + 0.001f > 0.0f)) && normalTemp.w != -1.0f) {
		/*LIGHTING ON BOTTOM OF THE OCEAN*/
		//Does not use specular.
		/*Note, attenuation is not relevant FOR THE SUN, and is as such not included in the calculations*/
		/*AMBIENT*/
		//Calculate ambient light:
		float3 finalAmbientColor = ambientColor * ambientLightIntensity;

		/*DIFFUSE*/
		//Calculate vector from surface position to light position:
		float3 surfaceToLightVector = lightPositionWS - wPos.xyz;

		//Calculate the direction from surface position to light position:
		float3 directionToLightVector = normalize(surfaceToLightVector);

		//Calculate the diffuse scalar depending on the angle at which the light strikes the surface.
		//Clamp the value between 0 and 1, inclusively:
		float diffuseScalar = saturate(dot(directionToLightVector, normal));

		//Final diffuse color, taking "everything diffuse" into consideration:
		float3 finalDiffuseColor = diffuseColor * diffuseLightIntensity * diffuseScalar;

		//We now return the final TOTAL color, taking into consideration the model color, ambient color, diffuse color and specular color contributions: 
        texCol = float4(saturate(finalAmbientColor + ((1 - shadow) * finalDiffuseColor)) * texCol.xyz, 1.0f);

		float4 groundCol = texCol;

		//How much water we are looking through.
		float oceanViewDepth = min(closestPlanet.y, depth - closestPlanet.x);
		
		//Removes artefacts around the ocean "line".
		if (oceanViewDepth < 0.0f) {
			oceanViewDepth = closestPlanet.y;
		}

		float opticalDepth = 1 - exp(-oceanViewDepth * 0.05f);
		float alpha = (1 - exp(-oceanViewDepth * 1.0f)) * clamp((((-depth) / 3000) + 4), 0, 1); // CHECK FOR MULTIPLICATION
		float4 oceanCol = lerp(float4(0.2f, 0.2f, 0.5f, 1.0f), float4(0.05f, 0.05f, 0.4f, 1.0f), opticalDepth);

		texCol = oceanCol;

		normal = normalize(wPos.xyz - centerPlanet.xyz);
		
		/*LIGHTING FOR WATER*/
		/*Note, attenuation is not relevant FOR THE SUN, and is as such not included in the calculations*/
		/*AMBIENT*/
		//Calculate ambient light:
		finalAmbientColor = ambientColor * ambientLightIntensity;

		/*DIFFUSE*/
		//Calculate vector from surface position to light position:
		surfaceToLightVector = lightPositionWS - wPos.xyz;

		//Calculate the direction from surface position to light position:
		directionToLightVector = normalize(surfaceToLightVector);

		//Calculate the diffuse scalar depending on the angle at which the light strikes the surface.
		//Clamp the value between 0 and 1, inclusively:
		diffuseScalar = saturate(dot(directionToLightVector, normal));

		//Final diffuse color, taking "everything diffuse" into consideration:
		finalDiffuseColor = diffuseColor * diffuseLightIntensity * diffuseScalar;

		/*SPECULAR*/
		//Direction vector from surface position towards light position:
		float3 surfaceTolightDirection = normalize(lightPositionWS - wPos.xyz);

		//Direction vector from surface position towards camera position:
		float3 surfaceToCameraDirection = normalize(cameraPos.xyz - wPos.xyz);

		//The halfway vector between the previous two vectors:
		float3 halfwayVector = normalize(surfaceTolightDirection + surfaceToCameraDirection);

		//The specular scalar constant given by the angle between the normal and haldway vector, raised to some power.
		//The lower the power is, the greater area of specular light, and vice versa:
		//Specular power is hard coded for the water.
		float specularScalar = pow(saturate(dot(normal, halfwayVector)), 300.0f);

		//The final specular color, using the diffuse color as its own color, also taking into account specular intensity:
		float3 finalSpecularColor = (diffuseColor * diffuseLightIntensity) * specularIntensity * specularScalar;

		//We now return the final TOTAL color, taking into consideration the model color, ambient color, diffuse color and specular color contributions: 
        texCol = float4(saturate(finalAmbientColor + ((1 - shadow) * finalDiffuseColor)) * texCol.xyz, 1.0f);

		texCol = lerp(groundCol, texCol, alpha);

        texCol.xyz = texCol.xyz + ((1 - shadow) * (finalSpecularColor * 0.5f));
    }
	else {
		/*LIGHTING FOR NORMAL GROUND*/
		/*Note, attenuation is not relevant FOR THE SUN, and is as such not included in the calculations*/
		/*AMBIENT*/
		//Calculate ambient light:
		float3 finalAmbientColor = ambientColor * ambientLightIntensity;

		/*DIFFUSE*/
		//Calculate vector from surface position to light position:
		float3 surfaceToLightVector = lightPositionWS - wPos.xyz;

		//Calculate the direction from surface position to light position:
		float3 directionToLightVector = normalize(surfaceToLightVector);

		//Calculate the diffuse scalar depending on the angle at which the light strikes the surface.
		//Clamp the value between 0 and 1, inclusively:
		float diffuseScalar = saturate(dot(directionToLightVector, normal));

		//Final diffuse color, taking "everything diffuse" into consideration:
		float3 finalDiffuseColor = diffuseColor * diffuseLightIntensity * diffuseScalar;

		/*SPECULAR*/
		//Direction vector from surface position towards light position:
		float3 surfaceTolightDirection = normalize(lightPositionWS - wPos.xyz);

		//Direction vector from surface position towards camera position:
		float3 surfaceToCameraDirection = normalize(cameraPos.xyz - wPos.xyz);

		//The halfway vector between the previous two vectors:
		float3 halfwayVector = normalize(surfaceTolightDirection + surfaceToCameraDirection);

		//The specular scalar constant given by the angle between the normal and haldway vector, raised to some power.
		//The lower the power is, the greater area of specular light, and vice versa:
		float specularScalar = pow(saturate(dot(normal, halfwayVector)), specularPower);

		//The final specular color, using the diffuse color as its own color, also taking into account specular intensity:
		float3 finalSpecularColor = (diffuseColor * diffuseLightIntensity) * specularIntensity * specularScalar;

		//We now return the final TOTAL color, taking into consideration the model color, ambient color, diffuse color and specular color contributions: 
        texCol = float4(saturate(finalAmbientColor + ((1 - shadow) * (finalDiffuseColor + finalSpecularColor))) * texCol.xyz, 1.0f);
    }
	return texCol;
}