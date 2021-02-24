TextureCube shadowMap : register(t0);
SamplerState pointSampler : register(s0);

cbuffer lightConstantBuffer : register(b0)
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

cbuffer cameraConstantBuffer : register(b1)
{
    float3 cameraPositionWS;
    float padding;
}

cbuffer shadowData : register(b2)
{
    float shadowBias;
    float3 shadowPadding;
}

struct PS_IN
{
    float4 inPositionSS : SV_Position;
    float3 inPositionWS : POSITION;
	float4 inColor	    : COLOR;
    float3 inNormalWS   : NORMAL;
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
    float3 normalWS = normalize(psIn.inNormalWS);
    
    /*SHADOW*/
    float shadow = CalculateShadowFactor(psIn.inPositionWS);
    
    /*LIGHTING*/
    /*Note, attenuation is not relevant FOR THE SUN, and is as such not included in the calculations*/
    /*AMBIENT*/
    //Calculate ambient light:
    float3 finalAmbientColor = ambientLightIntensity * ambientColor;
    
    /*DIFFUSE*/
    //Calculate  direction vector from surface position towards light position:
    float3 directionToLightVector = normalize(lightPositionWS - psIn.inPositionWS);
    
    //Calculate the diffuse scalar depending on the angle at which the light strikes the surface.
    //Clamp the value between 0 and 1, inclusively:
    float diffuseScalar = saturate(dot(directionToLightVector, normalWS));
    
    //Final diffuse color, taking "everything diffuse" into consideration:
    float3 finalDiffuseColor = diffuseColor * diffuseLightIntensity * diffuseScalar;
    
    /*SPECULAR*/
    //Direction vector from surface position towards light position:
    float3 surfaceTolightDirection = normalize(lightPositionWS - psIn.inPositionWS);
    
    //Direction vector from surface position towards camera position:
    float3 surfaceToCameraDirection = normalize(cameraPositionWS - psIn.inPositionWS);
    
    //The halfway vector between the previous two vectors:
    float3 halfwayVector = normalize(surfaceTolightDirection + surfaceToCameraDirection);
    
    //The specular scalar constant given by the angle between the normal and haldway vector, raised to some power.
    //The lower the power is, the greater area of specular light, and vice versa:
    float specularScalar = pow(saturate(dot(normalWS, halfwayVector)), specularPower);
    
    //The final specular color, using the diffuse color as its own color, also taking into account specular intensity:
    float3 finalSpecularColor = (diffuseColor * diffuseLightIntensity) * specularIntensity * specularScalar;
    
    //We now return the final TOTAL color, taking into consideration the model color, ambient color, diffuse color and specular color contributions: 
    float3 lighting = saturate((finalAmbientColor + (1.0 - shadow) * (finalDiffuseColor + finalSpecularColor)) * psIn.inColor.xyz);
    return float4(lighting, 1.0f);
}