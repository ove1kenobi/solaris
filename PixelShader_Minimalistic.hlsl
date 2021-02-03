struct PS_IN
{
    float4 inPositionSS : SV_Position;
	float4 inColorSS	: COLOR;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
	return psIn.inColorSS;
}