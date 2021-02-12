cbuffer Transforms : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
}

struct VS_IN
{
    float3 inPositionWS : POSITION; //Not a typo.
};

struct VS_OUT
{
    float3 outPositionWS : POSITION;
    float4 outPositionCS : SV_Position;
};

VS_OUT vs_main(in VS_IN vsIn) 
{
    VS_OUT vsOut = (VS_OUT)0;
    vsOut.outPositionWS = vsIn.inPositionWS;
    vsOut.outPositionCS = mul(mul(float4(vsIn.inPositionWS, 0.0f), viewMatrix), projectionMatrix);
    //Rasterizer will do a z/w to go from clip space to screen space. Setting z = w means w/w = 1.0f, effectively giving the depth value of 1.
    vsOut.outPositionCS.z = vsOut.outPositionCS.w;
	return vsOut;
}