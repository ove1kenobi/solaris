#pragma once
#include "vertex.h"
class Model
{
private:
	bool m_notLoaded;
	UINT m_stride;
	UINT m_offset;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_radiusBuffer;
	UINT m_vertexBufferSize;
	UINT m_indexBufferSize;
	DirectX::BoundingBox* m_boundingBox;
	DirectX::BoundingSphere* m_boundingSphere;
public:
	Model();
	~Model();
	bool NotLoaded();
	void Loaded();

	Microsoft::WRL::ComPtr<ID3D11Buffer>& getVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer>& getIndexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer>& getMatrixBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer>& getRadiusBuffer();

	void setIndexBufferSize(UINT size);
	void setVertexBufferSize(UINT size);
	UINT getIndexBufferSize();
	UINT getVertexBufferSize();

	void setStride(UINT stride);
	void setOffset(UINT offset);

	UINT& getStride();
	UINT& getOffset();
	void SetBoundingVolume(DirectX::BoundingBox* box);
	void SetBoundingVolume(DirectX::BoundingSphere* sphere);
	DirectX::BoundingBox* GetBoundingBox();
	DirectX::BoundingSphere* GetBoundingSphere();
};