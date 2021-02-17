#pragma once
#include "GameObject.h"

class SpaceShip : public GameObject
{
private:
	float m_pitchTilt, m_rollTilt;

	// Physics
	std::vector<DirectX::XMFLOAT3> m_forces;
	void UpdatePhysics();
public:
	SpaceShip();
	~SpaceShip() = default;

	void Move(float step);
	void AddRotation(float yaw, float pitch);
	void SetTilt(float pitchLerp, float rollLerp);
	void SetForwardVector(DirectX::XMFLOAT3 cameraPos);

	DirectX::XMFLOAT3 getCenter();
	virtual bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void CalculateGravity(GameObject* other);
	void AddForce(DirectX::XMFLOAT3 f);
};