#pragma once
#include "GameObject.h"

class Orbit : public GameObject
{
private:

public:
	Orbit() noexcept;
	virtual ~Orbit();

	bool init(float major_semi_axis, float minor_semi_axis);
	bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
};