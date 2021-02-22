cbuffer MatrixBuffer : register(b0) {
    matrix WMatrix;
    matrix VMatrix;
    matrix PMatrix;
}

cbuffer RadiusBuffer : register(b1) {
    float radius;
};

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
    float outLengthToCenter : LENGTHCENTER;
    float outRadius         : RADIUS;
    float4 outColor         : COLOR;
    float3 outNormalWS      : NORMAL;
};

VS_OUT vs_main(in VS_IN vsIn)
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outLengthToCenter = length(vsIn.inPositionLS); 
    
    vsOut.outPositionCS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix);
    vsOut.outPositionCS = mul(vsOut.outPositionCS, VMatrix);
    vsOut.outPositionCS = mul(vsOut.outPositionCS, PMatrix);

    vsOut.outPositionWS = mul(float4(vsIn.inPositionLS, 1.0f), WMatrix).xyz;
    
    vsOut.outColor = vsIn.inColor;

    vsOut.outRadius = radius;
    
    vsOut.outNormalWS = normalize(mul(float4(vsIn.inNormalLS, 0.0f), WMatrix).xyz);
    
    return vsOut;
}