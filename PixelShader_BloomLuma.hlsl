Texture2D textureToSample : register(t0);
SamplerState linearSampler : register(s0);

struct PS_IN
{
    float4 inPositionSS : SV_Position;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    float4 color = textureToSample.Sample(linearSampler, psIn.inPositionSS.xy);
    
	return color;
}