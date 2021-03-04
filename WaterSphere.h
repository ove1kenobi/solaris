#pragma once
#include "GameObject.h"

class WaterSphere : public GameObject {
private:
	float m_radius;
	std::string tag;
	bool testForCulling;
public:
	WaterSphere() noexcept;
	~WaterSphere() noexcept = default;

	bool Initialize(float x, float y, float z, float r);
	GameObject* update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);
	void updateSphere(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext, float x, float y, float z);
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);
	void BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	//Not used.
	const bool IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept {
		return true;
	}

	[[nodiscard]] const std::string& GetTag() const noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
};