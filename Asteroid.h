#pragma once
#include "GameObject.h"

class Asteroid : public GameObject
{
private:
	std::vector<DirectX::XMFLOAT3> m_forces;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_AmatrixBuffer;
public:
	Asteroid() noexcept;
	virtual ~Asteroid();

	bool init(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velocity);
	bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
	void CalculateGravity(GameObject* other);
	void AddForce(DirectX::XMFLOAT3 f);
	void UpdatePhysics();
};