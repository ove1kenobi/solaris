cbuffer MatrixBuffer 
{
    matrix WMatrix;
    matrix WVPMatrix;
}

struct VS_IN
{
    float3 inPositionLS : POSITION;
    float4 inColor      : COLOR;
    float3 inNormalLS   : NORMAL;
};

struct VS_OUT
{
    float4 outPositionCS    : SV_Position;
    float3 outPositionWS    : POSITION;
    float4 outColor         : COLOR;
    float3 outNormalWS      : NORMAL;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionCS = mul(float4(vsIn.inPositionLS, 1.0f), WVPMatrix);

    vsOut.outPositionWS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix).xyz;
    
    vsOut.outColor = vsIn.inColor;

    vsOut.outNormalWS = normalize(mul(float4(vsIn.inNormalLS, 0.0f), WMatrix).xyz);
    
    return vsOut;
}