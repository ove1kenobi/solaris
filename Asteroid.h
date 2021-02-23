#pragma once
#include "GameObject.h"

class Asteroid : public GameObject
{
private:
	float m_scale;
	float m_deltaRoll;
	float m_deltaYaw;
	float m_deltaPitch;
	GameObject* m_ship;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_AmatrixBuffer;
public:
	Asteroid() noexcept;
	virtual ~Asteroid();

	bool init(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velocity, GameObject* ship);
	GameObject* update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
};