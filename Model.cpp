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

void Model::AddVertex(vertex v)
{
	m_vertices.push_back(v);
}

void Model::AddIndex(UINT i)
{
	m_indices.push_back(i);
}

std::vector<vertex> Model::getVertexBuffer() {
	return this->m_vertices;
}

std::vector<UINT> Model::getIndexBuffer() {
	return this->m_indices;
}