#pragma once
#include "GameObject.h"
//Represents a planet, moon or asteroid.
class CosmicBody : public GameObject
{
private:
	float m_radius;
	DirectX::XMFLOAT4 m_yAxis;
public:
	CosmicBody() noexcept;
	~CosmicBody() = default;
	bool init(float x, float y, float z, float r, float xRot, float zRot);
	bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
};