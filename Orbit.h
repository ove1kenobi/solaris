#pragma once
#include "GameObject.h"

class Orbit : public GameObject
{
private:
	bool m_TestForCulling;
public:
	Orbit() noexcept;
	virtual ~Orbit();

	bool init(float major_semi_axis, float minor_semi_axis);
	GameObject* update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
};