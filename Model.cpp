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
}

void Model::AddIndex(UINT i)
{
	m_indices.push_back(i);
}
