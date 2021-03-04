Texture2D highlightBlurTexture	: register(t0);
Texture2D<float4> colorTexture	: register(t1);

struct PS_IN
{
    float4 inPositionSS : SV_Position;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    int3 colorTexelLocation = int3(psIn.inPositionSS.x, psIn.inPositionSS.y, 0);
    int3 HBTexelLocation = int3(psIn.inPositionSS.x, psIn.inPositionSS.y, 0);
	
    float4 sceneColor = colorTexture.Load(colorTexelLocation);
    float4 highlightColor = highlightBlurTexture.Load(HBTexelLocation);
	
    return saturate((sceneColor + highlightColor));
}