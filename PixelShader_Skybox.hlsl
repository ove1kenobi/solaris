TextureCube textureCube : register(t0);
SamplerState linearSampler : register(s0);

struct PS_IN
{
    float3 inPositionWS : POSITION;
    float4 inPositionSS : SV_Position;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    return textureCube.Sample(linearSampler, psIn.inPositionWS);

}