#include "Model.h"

Model::Model() : m_notLoaded{ true }
{

}

bool Model::NotLoaded()
{
	return m_notLoaded;
}

void Model::Loaded()
{
	m_notLoaded = false;
}

void Model::AddVertex(vertex_tex v)
{
	m_vertices.push_back(v);
	positions.push_back(v.position);
}

void Model::AddIndex(UINT i)
{
	m_indices.push_back(i);
}

float* Model::PosData()
{
	return (float*)positions.data();
}

UINT Model::DataLength()
{
	return positions.size() * sizeof(DirectX::XMFLOAT3);
}

UINT Model::NumVertices()
{
	return positions.size();
}