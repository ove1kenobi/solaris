#pragma once
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

struct Vertex_Position
{
	DirectX::XMFLOAT3 position;
};

struct Vertex_PosTex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
};