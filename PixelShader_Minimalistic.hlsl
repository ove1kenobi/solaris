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
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
    float specularIntensity;
}

struct PS_IN
{
    float4 inPositionSS : SV_Position;
    float3 inPositionWS : POSITION;
	float4 inColor	    : COLOR;
    float3 inNormalWS   : NORMAL;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    float3 normalWS = normalize(psIn.inNormalWS);
    
    /*LIGHTING*/
    /*Note, attenuation is not relevant FOR THE SUN, and is as such not included in the calculations*/
    /*AMBIENT*/
    //Calculate ambient light:
    float3 finalAmbientColor = ambientColor * ambientLightIntensity;
    
    /*DIFFUSE*/
    //Calculate vector from surface position to light position:
    float3 surfaceToLightVector = lightPositionWS - psIn.inPositionWS;
    
    //Calculate length of surfaceToLightVector:
    float distanceToLight = length(surfaceToLightVector);
    
    //Calculate the direction from surface position to light position:
    float3 directionToLightVector = normalize(surfaceToLightVector);
    
    //Calculate the diffuse scalar depending on the angle at which the light strikes the surface.
    //Clamp the value between 0 and 1, inclusively:
    float diffuseScalar = saturate(dot(directionToLightVector, normalWS));
    
    //Final diffuse color, taking "everything diffuse" into consideration:
    float3 finalDiffuseColor = diffuseColor * diffuseLightIntensity * diffuseScalar;
    
    //We now return the final TOTAL color, taking into consideration the model color, ambient color and diffuse color contributions: 
    return float4(saturate(finalAmbientColor + finalDiffuseColor) * psIn.inColor.xyz, 1.0f);
}