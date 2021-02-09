#pragma once
#include "Time.h"
#include "ModelFactory.h"
#include <imgui.h>


class GameObject
{
protected:
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_center;
	DirectX::XMFLOAT3 m_forwardVector;
	DirectX::XMFLOAT3 m_rightVector;
	DirectX::XMFLOAT3 m_upVector;
	DirectX::XMFLOAT4X4 m_wMatrix;
	float m_mass;
	float m_1byMass;
	float m_pitch;
	float m_roll;
	float m_yaw;
	Time m_timer;

	// Physics
	static const float G;

	//Where the buffers are stored
	Model* m_model;
public:
	std::vector<DirectX::XMFLOAT3> m_forces;
	GameObject() noexcept;
	~GameObject();

	//Forces all GameObjects (planets, sun, ship) to have an update function and a function to bind their buffers.
	virtual bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) = 0;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) = 0;

	void getWMatrix(DirectX::XMMATRIX& wMatrix);
	UINT getVertexBufferSize();
	UINT getIndexBufferSize();
	[[nodiscard]] const DirectX::XMFLOAT3& GetCenter() const noexcept;
	//Returns the translation of the object in the world.
	DirectX::XMFLOAT3 getTransVector();

	// Pysics
	void CalculateGravity(GameObject* other);
	void AddForce(DirectX::XMFLOAT3);
	void UpdatePhysics();
};