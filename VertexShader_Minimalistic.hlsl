cbuffer MatrixBuffer : register(b0) {
    matrix WMatrix;
    matrix VMatrix;
    matrix PMatrix;
}

cbuffer RadiusBuffer : register(b1) {
    float radius;
};

struct VS_IN
{
    float3 inPositionLS : POSITION;
    float4 inColorLS : COLOR;
    float3 inNormalLS : NORMAL;
};

struct VS_OUT
{
    float4 outPositionPS : SV_Position;
    float4 outPositionWS : WPOSITION;
    float outLengthToCenter : LENGTHCENTER;
    float outRadius : RADIUS;
    float4 outColorPS : COLOR;
    float4 outNormalPS : NORMAL;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outLengthToCenter = length(vsIn.inPositionLS);

    vsOut.outPositionPS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix);
    vsOut.outPositionPS = mul(vsOut.outPositionPS, VMatrix);
    vsOut.outPositionPS = mul(vsOut.outPositionPS, PMatrix);

    vsOut.outPositionWS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix);

    vsOut.outColorPS = vsIn.inColorLS;

    vsOut.outRadius = radius;

    vsOut.outNormalPS = mul(float4(vsIn.inNormalLS, 0.0f), WMatrix);
    return vsOut;
}