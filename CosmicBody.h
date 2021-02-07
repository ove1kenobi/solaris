#pragma once
#include "GameObject.h"
//Represents a planet, moon or asteroid.
class CosmicBody : public GameObject
{
private:
	float m_radius;
	DirectX::XMFLOAT4 m_yAxis;
	int m_rotationDir;
public:
	CosmicBody() noexcept;
	~CosmicBody() = default;
	//Sets all the randomized values.
	bool init(float x, float y, float z, float r, float xRot, float zRot, int rotDir);
	bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
};