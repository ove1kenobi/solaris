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
	bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);
	void updateSphere(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext, float x, float y, float z);
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);

	//Not used.
	const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept {
		return true;
	}

	[[nodiscard]] const std::string& GetTag() const noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
};