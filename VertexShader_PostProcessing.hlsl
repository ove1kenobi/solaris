cbuffer MatrixBuffer
{
    matrix WMatrix;
    matrix VMatrix;
    matrix PMatrix;
};

struct VS_IN
{
    float3 inPositionLS : POSITION;
    float2 inTexUVLS    : TEXUV;
};

struct VS_OUT
{
    float4 outPositionPS : SV_Position;
    float2 outTexUVPS    : TEXUV;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionPS = float4(vsIn.inPositionLS, 1.0f);
    //vsOut.outPositionPS = mul(vsOut.outPositionPS, WMatrix);
    //vsOut.outPositionPS = mul(vsOut.outPositionPS, VMatrix);
    //vsOut.outPositionPS = mul(vsOut.outPositionPS, PMatrix);

    vsOut.outTexUVPS = vsIn.inTexUVLS;

	return vsOut;
}