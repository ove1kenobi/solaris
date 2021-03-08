#pragma once
#include "GameObject.h"
#include "ourMath.h"
#include "SpaceShipUpgrade.h"

class SpaceShip : public GameObject
{
private:
	float m_pitchTilt, m_rollTilt;

	std::string m_Tag;
	bool m_TestForCulling;

	// Spaceship upgrades
	std::vector<GameObject*> m_upgrades;

public:
	SpaceShip();
	virtual ~SpaceShip();


	void AddRotation(float yaw, float pitch);
	void SetTilt(float pitchLerp, float rollLerp);
	void SetForwardVector(DirectX::XMFLOAT3 forwardVector);

	float GetTopSpeed();
	DirectX::XMFLOAT3 GetVelocity();
	DirectX::XMFLOAT3 getCenter();
	virtual GameObject* update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept override;
	void BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const std::string& GetTag() const noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
	std::vector<GameObject*>& GetActiveUpgrades();
};