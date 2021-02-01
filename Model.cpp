#include "Model.h"

Model::Model() : m_notLoaded{ true }
{

}

bool Model::NotLoaded()
{
	return m_notLoaded;
}

void Model::AddVertex(vertex v)
{
	m_vertices.push_back(v);
}
