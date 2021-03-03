cbuffer MatrixBuffer : register(b0) {
    matrix WMatrix;
    matrix WVPMatrix;
    
}

struct VS_IN
{
    float3 inPositionLS : POSITION;
};

struct VS_OUT
{
    float4 outPositionCS    : SV_Position;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;

    vsOut.outPositionCS = mul(float4(vsIn.inPositionLS, 1.0f), WVPMatrix);

    return vsOut;
}