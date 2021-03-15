#pragma once
#include "GameObject.h"
class SpaceShipUpgrade :
    public GameObject
{
private:
	std::string m_Tag;
	bool m_TestForCulling;
public:
	SpaceShipUpgrade();
	SpaceShipUpgrade(std::string modelFile);
	virtual ~SpaceShipUpgrade() = default;
	virtual GameObject* update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);
	virtual [[nodiscard]] const bool IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept;
	virtual void BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext);
	virtual [[nodiscard]] const std::string& GetTag() const noexcept;
	virtual [[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept;
};

