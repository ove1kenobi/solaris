#include "pch.h"
#include "Model.h"

Model::Model()
	: m_notLoaded{ true }, m_stride{ 0 }, m_offset{ 0 },
	m_indexBufferSize{ 0 }, m_vertexBufferSize{ 0 },
	m_boundingBox{ nullptr }, m_boundingSphere{ nullptr }
{

}

Model::~Model()
{
	if (m_boundingBox)
	{
		delete m_boundingBox;
		m_boundingBox = nullptr;
	}
	if (m_boundingSphere)
	{
		delete m_boundingSphere;
		m_boundingSphere = nullptr;
	}
}

bool Model::NotLoaded()
{
	return m_notLoaded;
}

void Model::Loaded()
{
	m_notLoaded = false;
}

Microsoft::WRL::ComPtr<ID3D11Buffer>& Model::getVertexBuffer() {
	return this->m_vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer>& Model::getIndexBuffer() {
	return this->m_indexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer>& Model::getMatrixBuffer() {
	return this->m_matrixBuffer;
}

void Model::setIndexBufferSize(UINT size) {
	this->m_indexBufferSize = size;
}

void Model::setVertexBufferSize(UINT size) {
	this->m_vertexBufferSize = size;
}

UINT Model::getIndexBufferSize() {
	return this->m_indexBufferSize;
}

UINT Model::getVertexBufferSize() {
	return this->m_vertexBufferSize;
}

void Model::setStride(UINT stride) {
	this->m_stride = stride;
}

void Model::setOffset(UINT offset) {
	this->m_offset = offset;
}

UINT& Model::getStride() {
	return this->m_stride;
}

UINT& Model::getOffset() {
	return this->m_offset;
}

void Model::SetBoundingVolume(DirectX::BoundingBox* box)
{
	m_boundingBox = box;
}

void Model::SetBoundingVolume(DirectX::BoundingSphere* sphere)
{
	m_boundingSphere = sphere;
}

DirectX::BoundingBox* Model::GetBoundingBox()
{
	return m_boundingBox;
}

DirectX::BoundingSphere* Model::GetBoundingSphere()
{
	return m_boundingSphere;
}

