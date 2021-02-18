cbuffer MatrixBuffer {
    matrix WMatrix;
    matrix VMatrix;
    matrix PMatrix;
}

struct VS_IN
{
    float3 inPositionLS : POSITION;
};

struct VS_OUT
{
    float4 outPositionPS : SV_Position;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionPS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix);
    vsOut.outPositionPS = mul(vsOut.outPositionPS, VMatrix);
    vsOut.outPositionPS = mul(vsOut.outPositionPS, PMatrix);

    return vsOut;
}