#pragma once
#include "GameObject.h"

class Asteroid : public GameObject
{
private:
	float m_deltaRoll;
	float m_deltaYaw;
	float m_deltaPitch;
	bool m_TestForCulling;
	GameObject* m_ship;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_AmatrixBuffer;
	bool m_IsDestroyed;
public:
	Asteroid() noexcept;
	virtual ~Asteroid();

	bool init(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velocity, GameObject* ship);
	GameObject* update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept override;
	virtual [[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept;
	virtual void BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	void SetVelocity(const DirectX::XMFLOAT3& velocity) noexcept;
	void MarkAsDestroyed() noexcept;
};