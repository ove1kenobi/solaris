struct particle
{
	float3 position;
	float size;
};

AppendStructuredBuffer<particle> NewSimState : register(u0);

cbuffer ParticleParameters
{
	float4 EmitterCenter;
	float4 RandomVector;
};

static const float EmitterRadius = 9.0f;

static const float3 direction[8] =
{
	normalize(float3( 1.0f,  1.0f,  1.0f)) * EmitterRadius,
	normalize(float3(-1.0f,  1.0f,  1.0f)) * EmitterRadius,
	normalize(float3(-1.0f, -1.0f,  1.0f)) * EmitterRadius,
	normalize(float3( 1.0f, -1.0f,  1.0f)) * EmitterRadius,
	normalize(float3( 1.0f,  1.0f, -1.0f)) * EmitterRadius,
	normalize(float3(-1.0f,  1.0f, -1.0f)) * EmitterRadius,
	normalize(float3(-1.0f, -1.0f, -1.0f)) * EmitterRadius,
	normalize(float3( 1.0f, -1.0f, -1.0f)) * EmitterRadius
};

[numthreads(8, 1, 1)]

void main( uint3 GroupThreadID : SV_GroupThreadID)
{
	particle p;

	// Initialize positions to a sphere with EmitterRadius
	p.position = reflect(direction[GroupThreadID.x], RandomVector.xyz);

	// Move starting sphere to around EmitterCenter
	p.position += EmitterCenter.xyz;

	p.size = RandomVector.w;

	NewSimState.Append(p);
}