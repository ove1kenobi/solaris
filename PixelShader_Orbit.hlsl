struct PS_IN
{
    float4 inPositionSS : SV_Position;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    return float4(0.1, 0.1, 0.2, 1.0);
}