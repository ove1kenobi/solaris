Texture2D<float4> inputTexture      : register(t0);
RWTexture2D<float4> outputTexture   : register(u0);

/*The chosen filter kernel coefficient, as described in 
  Practical computation and rendering with DirectX 11*/
static const float filter[7][7] =
{
    0.000904706, 0.003157733, 0.00668492, 0.008583607, 0.00668492, 0.003157733, 0.000904706,
    0.003157733, 0.01102157, 0.023332663, 0.029959733, 0.023332663, 0.01102157, 0.003157733,
    0.00668492, 0.023332663, 0.049395249, 0.063424755, 0.049395249, 0.023332663, 0.00668492,
    0.008583607, 0.029959733, 0.063424755, 0.081438997, 0.063424755, 0.029959733, 0.008583607,
    0.00668492, 0.023332663, 0.049395249, 0.063424755, 0.049395249, 0.023332663, 0.00668492,
    0.003157733, 0.01102157, 0.023332663, 0.029959733, 0.023332663, 0.01102157, 0.003157733,
    0.000904706, 0.003157733, 0.00668492, 0.008583607, 0.00668492, 0.003157733, 0.000904706
};


//Hard coded for MY resolution as of now, should be dynamic of course.
[numthreads(32, 32, 1)]
void cs_main( uint3 DTid : SV_DispatchThreadID)
{
    //The offset texture location to sample from first:
    int3 textureLocation = DTid - int3(3, 3, 0);
    
    //Initialize the output color to black:
    float4 outputColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //Loop through all filter samples, then apply to the sampled texel, then sum:  
    for (int x = 0; x < 7; x++)
    {
        for (int y = 0; y < 7; y++)
        {
            outputColor += inputTexture.Load(textureLocation + int3(x, y, 0)) * filter[x][y];

        }
    }
    outputTexture[DTid.xy] = outputColor;
}