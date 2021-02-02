#pragma once
#include <vector>
#include <Windows.h>
#include <DirectXMath.h>

struct vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
};

struct vertex_tex : public vertex
{
	DirectX::XMFLOAT2 texcoord;
};

struct vertex_col : public vertex
{
	DirectX::XMFLOAT4 color;
};

class Model
{
private:
	bool m_notLoaded;
	std::vector<vertex> m_vertices;
	std::vector<UINT> m_indices;
public:
	Model();
	~Model() = default;
	bool NotLoaded();
	void AddVertex(vertex v);
	void AddIndex(UINT i);
	void Loaded();

	std::vector<vertex> getVertexBuffer();
	std::vector<UINT> getIndexBuffer();
};