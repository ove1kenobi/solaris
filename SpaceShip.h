#pragma once
#include "GameObject.h"

class SpaceShip : public GameObject
{
private:

public:
	SpaceShip();
	~SpaceShip() = default;
	virtual bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
};