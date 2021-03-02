struct particle
{
	float3 position;
	float size;
};

StructuredBuffer<particle> SimState;

struct VS_INPUT
{
	uint vertexID : SV_VertexID;
};

struct GS_INPUT
{
	float3 position : POSITION
};

GS_INPUT main( in VS_INPUT input )
{
	GS_INPUT output;

	output.position = SimState[input.vertexID].position;
	
	return output;
}