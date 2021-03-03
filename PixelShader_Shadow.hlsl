
static const float farPlane = 100000.0f;

cbuffer LightData : register(b0)
{
    float3 lightPosition;
    float padding;
}

struct PS_IN
{
    float4 inPositionSS : SV_Position;
    float3 inPositionWS : POSITION;
};

struct PS_OUT
{
    float4 outColor : SV_Target;
};

PS_OUT ps_main(in PS_IN psIn)
{
    PS_OUT psOut = (PS_OUT)0;
    float lightDistance = length(psIn.inPositionWS - lightPosition); 
    lightDistance = lightDistance / farPlane;
    psOut.outColor.r = lightDistance; 
    return psOut;
}