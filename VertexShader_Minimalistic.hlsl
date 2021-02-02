cbuffer MatrixBuffer {
    matrix WMatrix;
    matrix VMatrix;
    matrix PMatrix;
}

struct VS_IN
{
    float3 inPositionLS : POSITION;
    float3 inColorLS : COLOR;
};

struct VS_OUT
{
    float4 outPositionPS : SV_Position;
    float4 outColorPS : COLOR;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionPS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix);
    vsOut.outPositionPS = mul(vsOut.outPositionPS, VMatrix);
    vsOut.outPositionPS = mul(vsOut.outPositionPS, PMatrix);

    vsOut.outColorPS = float4(vsIn.inColorLS, 1.0f);
    return vsOut;
}