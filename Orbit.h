#pragma once
#include "GameObject.h"

class Orbit : public GameObject
{
private:

public:
	Orbit();
	virtual ~Orbit();

	bool init(float major_semi_axis, float minor_semi_axis);
	bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
};