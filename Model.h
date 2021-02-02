#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <DirectXMath.h>

struct vertex
{
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
	std::vector<DirectX::XMFLOAT3> positions;	// TODO: for testing, remove later
public:
	Model();
	~Model() = default;
	bool NotLoaded();
	void AddVertex(vertex_tex v);
	void AddIndex(UINT i);
	void Loaded();
	float* PosData();
	UINT DataLength();
	UINT NumVertices();
};