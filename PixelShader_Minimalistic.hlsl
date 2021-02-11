struct PS_IN
{
    float4 inPositionSS : SV_Position;
	float4 inColorSS	: COLOR;
    float4 inNormalSS   : NORMAL;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    float4 normal = normalize(psIn.inNormalSS);
    return psIn.inColorSS;
}