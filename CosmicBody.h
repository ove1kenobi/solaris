#pragma once
#include "GameObject.h"
#include "Orbit.h"
//Represents a planet, moon or asteroid.
class CosmicBody : public GameObject
{
private:
	DirectX::XMFLOAT4 m_yAxis;
	int m_rotationDir;
protected:
	float m_radius;
	GameObject* m_tetheredTo;
	float m_major_semi_axis;
	float m_minor_semi_axis;
	float m_orbital_speed;
	Time m_time;
	Orbit* m_orbit;
public:
	CosmicBody() noexcept;
	virtual ~CosmicBody();
	//Sets all the randomized values.
	bool init(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo, Orbit* orbit);
	GameObject* update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	GameObject* GetOrbit();
};