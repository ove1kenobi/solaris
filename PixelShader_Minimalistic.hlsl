struct PS_IN
{
    float4 inPositionSS     : SV_Position;
    float3 inPositionWS     : POSITION;
    float inLengthToCenter  : LENGTHCENTER;
    float inRadius          : RADIUS;
	float4 inColor	        : COLOR;
    float3 inNormalWS       : NORMAL;
};

struct PS_OUT
{
    float4 color            : SV_target0;
    float4 water            : SV_target1;
    float4 lengthToCenter   : SV_target2;
    float4 wPos             : SV_target3;
    float4 normal           : SV_target4;
};

PS_OUT ps_main(in PS_IN psIn)
{
    PS_OUT psOut = (PS_OUT)0;

    psOut.color = psIn.inColor;

    if (psIn.inLengthToCenter <= psIn.inRadius) {
        psOut.water = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        psOut.water = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    psOut.lengthToCenter = float4(psIn.inLengthToCenter, 0.0f, 0.0f, 1.0f);

    psOut.wPos = float4(psIn.inPositionWS, 1.0f);

    psOut.normal = float4(normalize(psIn.inNormalWS), 0.0f);
    return psOut;
}