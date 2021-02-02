struct PS_IN
{
    float4 inPositionSS : SV_Position;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
	return float4(0.0f, 0.8f, 0.5f, 1.0f);
}