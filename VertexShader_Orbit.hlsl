cbuffer MatrixBuffer {
    matrix WMatrix;
    matrix WVPMatrix;
}

struct VS_IN
{
    float3 inPositionLS : POSITION;
};

struct VS_OUT
{
    float4 outPositionPS : SV_Position;
    float4 outColor: COLOR;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    float distance = length(vsIn.inPositionLS);
    vsOut.outColor = float4(0.1, 0.1, 0.1, 1.0);
    if (distance < 6000.0) vsOut.outColor.x *= 3.0;
    if (distance > 15000.0) vsOut.outColor.z *= 3.0;
    
    vsOut.outPositionPS = mul(float4(vsIn.inPositionLS, 1.0f), WVPMatrix);

    return vsOut;
}