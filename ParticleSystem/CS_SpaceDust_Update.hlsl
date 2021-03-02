struct particle
{
	float3 position;
	float size;
};

AppendStructuredBuffer<Particle> NewSimState : register(u0);
ConsumeStructuredBuffer<Particle> CurrentSimState : register(u1);

static const EventHorizon = 10.0f;

cbuffer ParticleParameters
{
	float4 EmitterCenter;
	float4 RandomVector;
	float4 deltaTimeVelocity;
};

cbuffer ParticleCount
{
	float4 NumParticles;
};


[numthreads(512, 1, 1)]

void main( uint3 DTid : SV_DispatchThreadID )
{
	// Check if thread is outside bounds
	uint ThreadID = DTid.x + DTid.y * 512 + DTid.z * 512 * 512;

	if (ThreadID < NumParticles.x)
	{
		// Get the current particle
		Particle p = CurrentSimState.Consume();

		// Calculate new position
		p.position = p.postition + deltaTimeVelocity.xyz;

		// Append particle if inside event horizon
		if (length(p.position) < EventHorizon)
		{
			NewSimState.Append(p);
		}
	}
}