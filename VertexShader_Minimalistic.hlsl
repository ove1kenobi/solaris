struct VS_IN
{
    float3 inPositionLS : POSITION;
};

struct VS_OUT
{
    float4 outPositionCS : SV_Position;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionCS = float4(vsIn.inPositionLS, 1.0f);
    return vsOut;
}