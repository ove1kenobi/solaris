struct PS_IN
{
    float4 inPositionSS     : SV_Position;
    float3 inPositionWS     : POSITION;
    float inLengthToCenter  : LENGTHCENTER;
	float4 inColor	        : COLOR;
    float3 inNormalWS       : NORMAL;
};

struct PS_OUT
{
    float4 color            : SV_target0;
    float4 wPos             : SV_target1;
    float4 normal           : SV_target2;
};

PS_OUT ps_main(in PS_IN psIn)
{
    PS_OUT psOut = (PS_OUT)0;

    psOut.color = psIn.inColor;

    psOut.wPos = float4(psIn.inPositionWS, psIn.inLengthToCenter);

    psOut.normal = float4(normalize(psIn.inNormalWS), 0.0f);
    return psOut;
}

/* cbuffer cameraConstantBuffer : register(b1)
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
    closestTexelDepth *=
farPlane;
    /*Get the current depth of the surface as seen from the light being used now in PS*/
float currentSurfaceDepth = length(surfaceToLight);
    /*Is the current depth as seen from the light (minus bias) greater than the depth saved in the texture?
      If so, this specific surface is in shadow, else it's not!*/
float shadow = currentSurfaceDepth - shadowBias > closestTexelDepth ? 1.0f : 0.0f;
    return
shadow;
float3 lighting = saturate((finalAmbientColor + (1.0 - shadow) * (finalDiffuseColor + finalSpecularColor)) * psIn.inColor.xyz);
TextureCube shadowMap : register(t0);
SamplerState pointSampler : register(s0);
}*/