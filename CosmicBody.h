#pragma once
#include "GameObject.h"
#include "Orbit.h"
#include "WaterSphere.h"
//Represents a planet, moon or asteroid.
class CosmicBody : public GameObject
{
private:
	int m_rotationDir;
protected:
	DirectX::XMFLOAT4 m_yAxis;
	float m_radius;
	GameObject* m_tetheredTo;
	float m_major_semi_axis;
	float m_minor_semi_axis;
	float m_orbital_speed;
	Time m_time;
	Orbit* m_orbit;
	WaterSphere* m_waterSphere;
public:
	CosmicBody() noexcept;
	virtual ~CosmicBody();
	//Sets all the randomized values.
	bool init(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo, Orbit* orbit, WaterSphere* waterSphere);
	GameObject* update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) override;
	GameObject* GetOrbit();
	float GetRadius();
	int GetRotDir();
	[[nodiscard]] const float& GetOrbitalSpeed() const noexcept;
};