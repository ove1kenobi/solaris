#pragma once
#include "vertex.h"
#include "ModelTexture.h"


class Model
{
private:
	bool m_notLoaded;
	UINT m_stride;
	UINT m_offset;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
	std::vector<ModelTexture*> m_texture;
	UINT m_vertexBufferSize;
	UINT m_indexBufferSize;
	DirectX::BoundingBox* m_boundingBox;
	DirectX::BoundingSphere* m_boundingSphere;
public:
	Model();
	virtual ~Model();
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
	void SetBoundingVolume(DirectX::BoundingBox* box);
	void SetBoundingVolume(DirectX::BoundingSphere* sphere);
	DirectX::BoundingBox* GetBoundingBox();
	DirectX::BoundingSphere* GetBoundingSphere();
	std::vector<ModelTexture*>& GetTextures();
	void AddTexture(ModelTexture* tex, UINT type);

	enum TextureType {
		diffuse, diffuse2, numTextures
	};
};