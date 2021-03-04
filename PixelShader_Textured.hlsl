Texture2D diffuseTex         : register(t0);
SamplerState linearSampler : register(s0);

struct PS_IN
{
    float4 inPositionSS : SV_Position;
    float3 inPositionWS : POSITION;
    float2 inTexCoords  : TEXCOORD;
    float3 inNormalWS   : NORMAL;
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

    psOut.color = diffuseTex.Sample(linearSampler, psIn.inTexCoords);

    psOut.wPos = float4(psIn.inPositionWS, 0.0f);

    psOut.normal = float4(normalize(psIn.inNormalWS), -1.0f);
    return psOut;
}