#pragma once
#include <DirectXMath.h>
#include <vector>

#include "Time.h"
#include "ModelFactory.h"
#include "Model.h"
#include "DXDebug.h"

class GameObject
{
protected:
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_center;

	DirectX::XMFLOAT4X4 m_wMatrix;

	float m_mass;
	float m_pitch;
	float m_roll;
	float m_yaw;

	Time m_timer;

	//Where the buffers are stored
	Model* m_model;

public:
	GameObject() noexcept;
	~GameObject();

	virtual bool update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) = 0;
	virtual void bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) = 0;
	void getWMatrix(DirectX::XMMATRIX& wMatrix);
	UINT getVertexBufferSize();
	UINT getIndexBufferSize();

	[[nodiscard]] const DirectX::XMFLOAT3& GetCenter() const noexcept;
	DirectX::XMFLOAT3 getTransVector();
};

