Texture2DMS<float4> colorTexture : register(t0);
SamplerState SampleTypePoint : register(s0); 

struct PS_IN
{
	float4 outPositionPS : SV_Position;
	float2 outTexUVPS    : TEXUV;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
	float4 texCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		texCol += colorTexture.Load(psIn.outPositionPS.xy, i);
	}
	texCol = texCol / 4;

	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}