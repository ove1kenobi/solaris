#pragma once
#include "GameObject.h"

class SpaceShip : public GameObject
{
private:
	float m_rotationAngle;
	float pi;
public:
	SpaceShip();
	~SpaceShip() = default;

	void move(DirectX::XMFLOAT4 deltaPos);
	void rotate(float step);

	DirectX::XMFLOAT3 getCenter();
	virtual bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void CalculateGravity(GameObject* other);
	void AddForce(DirectX::XMFLOAT3 f);
	void UpdatePhysics();
};