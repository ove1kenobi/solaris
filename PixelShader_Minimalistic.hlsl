struct PS_IN
{
    float4 inPositionSS : SV_Position;
    float4 inPositionWS : WPOSITION;
    float inLengthToCenter : LENGTHCENTER;
    float inRadius : RADIUS;
	float4 inColorSS	: COLOR;
    float4 inNormalSS   : NORMAL;
};

struct PS_OUT
{
    float4 color : SV_target0;
    float4 water : SV_target1;
    float4 lengthToCenter : SV_target2;
    float4 wPos : SV_target3;
};

PS_OUT ps_main(in PS_IN psIn) : SV_TARGET
{
    PS_OUT psOut = (PS_OUT)0;
    float4 normal = normalize(psIn.inNormalSS);

    psOut.color = psIn.inColorSS;

    if (psIn.inLengthToCenter <= psIn.inRadius) {
        psOut.water = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        psOut.water = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    psOut.lengthToCenter = float4(psIn.inLengthToCenter, 0.0f, 0.0f, 1.0f);

    psOut.wPos = psIn.inPositionWS;

    return psOut;
}