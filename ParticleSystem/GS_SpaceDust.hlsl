cbuffer MatrixBuffer : register(b0) {
	matrix WVMatrix;
	matrix PMatrix;
}

cbuffer ParticleRenderParameters
{
	float4 EmitterLocation;
};

static const float EventHorizon = 10.0f;

static const float4 g_positions[4] =
{
	float4(-1,  1, 0, 0),
	float4( 1,  1, 0, 0),
	float4(-1, -1, 0, 0),
	float4( 1, -1, 0, 0)
};

static const float2 g_texcoords[4] =
{
	float2(0, 1),
	float2(1, 1),
	float2(0, 0),
	float2(1, 0)
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoords : TEXCOORDS;
	float4 color : COLOT;
};

struct GS_INPUT
{
	float3 position : POSITION;
};

[maxvertexcount(4)]

void main(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> SpriteStream)
{
	PS_INPUT output;

	float3 emitterToPos = EmitterLocation.xyx - input[0].position;
	float3 nearestEventHorizon = normalize(emitterToPos) * EventHorizon;
	float dist = saturate(length(nearestEventHorizon - emitterToPos) / 100.0f);
	float4 color = float4(0.2f, 0.2f, 1.0f, 0.0f) * dist + float4(1.0f, 0.2f, 0.2f, 0.0f) * (1.0f - dist);

	// Transform to view space
	float viewPosition = mul(float4(input[0].position, 1.0f), WVMatrix);

	// Emit two new triangles
	for (uint i = 0; i < 4; i++)
	{
		// Transform to clip space
		output.position = mul(viewPosition + g_positions[i], PMatrix);
		output.texcoords = g_texcoords[i];
		output.color = color;
		
		SpriteStream.Append(output);
	}
	SpriteStream.RestartStrip();
}