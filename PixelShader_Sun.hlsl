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
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    color = dot(normalize(color.xyz), normalize(psIn.inNormalWS));
    return saturate(color + psIn.inColor);
}