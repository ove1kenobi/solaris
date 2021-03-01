cbuffer matrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix worldViewProjectionMatrix;
};

struct VS_IN
{
    float3 inPositionLS : POSITION;
};

struct VS_OUT
{
    float4 outPositionCS : SV_Position;
    float3 outPositionWS : POSITION;
};

VS_OUT vs_main( in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionWS = mul(float4(vsIn.inPositionLS, 1.0f), worldMatrix).xyz;
    vsOut.outPositionCS = mul(float4(vsIn.inPositionLS, 1.0f), worldViewProjectionMatrix);
    return vsOut;
}