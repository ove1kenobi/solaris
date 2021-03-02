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