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

struct PS_IN
{
    float4 inPositionSS     : SV_Position;
    float3 inPositionWS     : POSITION;
    float inLengthToCenter  : LENGTHCENTER;
    float inRadius          : RADIUS;
	float4 inColor	        : COLOR;
    float3 inNormalWS       : NORMAL;
};

struct PS_OUT
{
    float4 color            : SV_target0;
    float4 water            : SV_target1;
    float4 lengthToCenter   : SV_target2;
    float4 wPos             : SV_target3;
};

PS_OUT ps_main(in PS_IN psIn) : SV_TARGET
{
    PS_OUT psOut = (PS_OUT)0;

    float3 normalWS = normalize(psIn.inNormalWS);
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
    psOut.color = float4(saturate(finalAmbientColor + finalDiffuseColor + finalSpecularColor) * psIn.inColor.xyz, 1.0f);

    if (psIn.inLengthToCenter <= psIn.inRadius) {
        psOut.water = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        psOut.water = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    psOut.lengthToCenter = float4(psIn.inLengthToCenter, 0.0f, 0.0f, 1.0f);

    psOut.wPos = float4(psIn.inPositionWS, 1.0f);

    return psOut;
}