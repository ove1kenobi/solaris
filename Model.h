#pragma once
#include <vector>
#include <DirectXMath.h>

struct vertex_tex
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
};

struct vertex_col
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
};

class Model
{
private:
	bool m_notLoaded;
	std::vector<vertex> m_vertices;
	std::vector<DWORD> m_indices;
public:
	Model();
	~Model() = default;
	bool NotLoaded();
	void AddVertex(vertex v);
};