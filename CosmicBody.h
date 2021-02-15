#pragma once
#include "GameObject.h"
//Represents a planet, moon or asteroid.
class CosmicBody : public GameObject
{
private:
	DirectX::XMFLOAT4 m_yAxis;
	int m_rotationDir;
protected:
	float m_radius;
	GameObject* m_tetheredTo;
	float m_major_semi_axix;
	float m_minor_semi_axix;
	float m_orbital_speed;
	Time m_time;
public:
	CosmicBody() noexcept;
	virtual ~CosmicBody();
	//Sets all the randomized values.
	bool init(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo);
	bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
	void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) override;
};