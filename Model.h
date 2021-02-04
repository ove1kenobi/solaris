#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <DirectXMath.h>
#include "DXDebug.h"

struct vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
};

struct vertex_tex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texcoord;
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
	
	UINT m_stride;
	UINT m_offset;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;

	UINT m_vertexBufferSize;
	UINT m_indexBufferSize;
public:
	Model();
	~Model() = default;
	bool NotLoaded();
	void Loaded();

	Microsoft::WRL::ComPtr<ID3D11Buffer>& getVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer>& getIndexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer>& getMatrixBuffer();

	void setIndexBufferSize(UINT size);
	void setVertexBufferSize(UINT size);
	UINT getIndexBufferSize();
	UINT getVertexBufferSize();

	void setStride(UINT stride);
	void setOffset(UINT offset);
	UINT& getStride();
	UINT& getOffset();
};