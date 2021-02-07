#include "pch.h"
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