#pragma once
#include <vector>
#include <Windows.h>
#include <DirectXMath.h>

struct vertex_tex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
};

struct vertex_col
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
};

class Model
{
private:
	bool m_notLoaded;
	std::vector<vertex_tex> m_vertices;
	std::vector<UINT> m_indices;
public:
	Model();
	~Model() = default;
	bool NotLoaded();
	void AddVertex(vertex_tex v);
	void AddIndex(UINT i);
	void Loaded();
};