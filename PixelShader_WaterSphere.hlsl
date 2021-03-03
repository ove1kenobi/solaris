struct PS_IN
{
    float4 inPositionSS : SV_Position;
};

struct PS_OUT
{
    float4 waterSphere  : SV_target3;
};

PS_OUT ps_main(in PS_IN psIn)
{
    PS_OUT psOut = (PS_OUT)0;

    psOut.waterSphere = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return psOut;
}