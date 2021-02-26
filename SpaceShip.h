#pragma once
#include "GameObject.h"
#include "ourMath.h"

class SpaceShip : public GameObject
{
private:
	float m_pitchTilt, m_rollTilt;

public:
	SpaceShip();
	virtual ~SpaceShip() = default;


	void AddRotation(float yaw, float pitch);
	void SetTilt(float pitchLerp, float rollLerp);
	void SetForwardVector(DirectX::XMFLOAT3 forwardVector);

	float GetTopSpeed();
	DirectX::XMFLOAT3 GetVelocity();
	DirectX::XMFLOAT3 getCenter();
	virtual GameObject* update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;

};