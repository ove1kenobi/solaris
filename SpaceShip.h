#pragma once
#include "GameObject.h"
#include "ourMath.h"

class SpaceShip : public GameObject
{
private:
	float m_pitchTilt, m_rollTilt;
	float m_topSpeed;

	std::string m_Tag;
	bool m_TestForCulling;
	// Physics
	std::vector<DirectX::XMFLOAT3> m_forces;
public:
	SpaceShip();
	virtual ~SpaceShip() = default;

	void UpdatePhysics();
	void AddRotation(float yaw, float pitch);
	void SetTilt(float pitchLerp, float rollLerp);
	void SetForwardVector(DirectX::XMFLOAT3 forwardVector);

	float GetTopSpeed();
	DirectX::XMFLOAT3 GetVelocity();
	DirectX::XMFLOAT3 getCenter();
	virtual bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
	void CalculateGravity(GameObject* other);
	void AddForce(DirectX::XMFLOAT3 f);
	[[nodiscard]] const std::string& GetTag() const noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
};