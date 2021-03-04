Texture2D<float4> inputTexture : register(t0);
RWTexture2D<float4> outputTexture : register(u0);

/*The chosen filter kernel coefficient, as described in 
  Practical computation and rendering with DirectX 11*/
static const float filter[7] = { 0.030078323, 0.104983664, 0.222250419, 0.285375187, 0.222250419, 0.104983664, 0.030078323 };

//The group shared memory to hold the loaded data:
groupshared float4 horizontalPoints[520];

//Hard coded for MY resolution as of now, should be dynamic of course.
[numthreads(1, 520, 1)]
void cs_main( uint3 DTid : SV_DispatchThreadID )
{
    //Load the data from the input texture:
    float4 data = inputTexture.Load(DTid);

    //Store the data into the group shared memory for the current thread:
    horizontalPoints[DTid.y] = data;
    
    //Now we wait for thread synchronization:
    GroupMemoryBarrierWithGroupSync();
    
    //We offset the texture location to the first sample location:
    int3 textureLocation = DTid - int3(0, 3, 0);
    
    //Initialize the final color to 0:
    float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //Loop over the 7 filter samples, apply them to the samples
    // and sum all results for the final color value:
    for (int y = 0; y < 7; y++)
    {
        outColor += horizontalPoints[textureLocation.y + y] * filter[y];
    }
    //Output the final color:
    outputTexture[DTid.xy] = outColor;
}