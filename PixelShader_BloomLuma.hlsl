Texture2D textureToSample : register(t0);

struct PS_IN
{
    float4 inPositionSS : SV_Position;
};

float4 ps_main(in PS_IN psIn) : SV_TARGET
{
    int3 location = int3(psIn.inPositionSS.x, psIn.inPositionSS.y, 0);
    float4 color = textureToSample.Load(location);
    float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
    if (brightness > 0.6 && brightness < 0.9)
    {
        return color;
    }
    else
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}