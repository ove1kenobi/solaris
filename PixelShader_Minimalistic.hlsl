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
    float bias;
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

float ShadowCalculations(in float3 positionWS)
{
    float3 surfaceToLight = positionWS - lightPositionWS;
    float closestDepth = shadowMap.Sample(pointSampler, surfaceToLight).r;
    closestDepth *= farPlane;
    float currentDepth = length(surfaceToLight);
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    return shadow;
}

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    float3 normalWS = normalize(psIn.inNormalWS);
    
    /*SHADOW*/
    float shadow = ShadowCalculations(psIn.inPositionWS);
    
    /*LIGHTING*/
    /*Note, attenuation is not relevant FOR THE SUN, and is as such not included in the calculations*/
    /*AMBIENT*/
    //Calculate ambient light:
    float3 finalAmbientColor = ambientColor * ambientLightIntensity;
    
    /*DIFFUSE*/
    //Calculate vector from surface position to light position:
    float3 surfaceToLightVector = lightPositionWS - psIn.inPositionWS;
    
    //Calculate the direction from surface position to light position:
    float3 directionToLightVector = normalize(surfaceToLightVector);
    
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
    //return float4(saturate((finalAmbientColor * (1 - shadow)) + finalDiffuseColor + finalSpecularColor) * psIn.inColor.xyz, 1.0f);
    float3 lighting = (finalAmbientColor + (1.0 - shadow) * (finalDiffuseColor + finalSpecularColor)) * psIn.inColor.xyz;
    return float4(lighting, 1.0f);
}