cbuffer MatrixBuffer 
{
    matrix WMatrix;
    matrix VMatrix;
    matrix PMatrix;
}

struct VS_IN
{
    float3 inPositionLS  : POSITION;
    float2 inTexCoord    : TEXCOORD;
    float3 inNormalLS    : NORMAL;
    float3 inTangentLS   : TANGENT;
    float3 inBitangentLS : BITANGENT;
};

struct VS_OUT
{
    float4 outPositionCS    : SV_Position;
    float3 outPositionWS    : POSITION;
    float2 outTexCoord      : TEXCOORD;
    float3 outNormalWS      : NORMAL;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionCS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix);
    vsOut.outPositionCS = mul(vsOut.outPositionCS, VMatrix);
    vsOut.outPositionCS = mul(vsOut.outPositionCS, PMatrix);

    vsOut.outPositionWS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix).xyz;
    
    vsOut.outTexCoord = vsIn.inTexCoord;

    vsOut.outNormalWS = normalize(mul(float4(vsIn.inNormalLS, 0.0f), WMatrix).xyz);

    return vsOut;
}