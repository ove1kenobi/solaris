struct VS_IN
{
    float3 inPositionLS : POSITION;
    float2 inTexUVLS    : TEXUV;
};

struct VS_OUT
{
    float4 outPositionPS : SV_Position;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionPS = float4(vsIn.inPositionLS, 1.0f);

	return vsOut;
}