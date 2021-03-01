struct PS_IN
{
    float4 inPositionSS     : SV_Position;
    float3 inPositionWS     : POSITION;
    float inLengthToCenter  : LENGTHCENTER;
    float4 inColor	        : COLOR;
    float3 inNormalWS       : NORMAL;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    return psIn.inColor;
}