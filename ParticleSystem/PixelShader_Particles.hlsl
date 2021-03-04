Texture2D ParticleTex : register(t0);
SamplerState LinearSampler : register(s0);

struct PS_INPUT
{
	float4 positoin : SV_POSITION;
	float2 texcoords : TEXCOORDS;
	float4 color : COLOR;
};

float4 main(in PS_INPUT input) : SV_TARGET
{
	float4 color = ParticleTex.Sample(LinearSampler, input.texcoords);
	color = color * input.color;

	return color;
}