cbuffer matrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_IN
{
    float3 inPositionWS : POSITION;
};

struct VS_OUT
{
    float4 outPositionCS : SV_Position;
    float3 outPositionWS : POSITION;
};

VS_OUT vs_main( in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionCS = mul(float4(vsIn.inPositionWS, 1.0f), worldMatrix);
    vsOut.outPositionWS = vsOut.outPositionCS.xyz;
    vsOut.outPositionCS = mul(vsOut.outPositionCS, viewMatrix);
    vsOut.outPositionCS = mul(vsOut.outPositionCS, projectionMatrix);    
    return vsOut;
}