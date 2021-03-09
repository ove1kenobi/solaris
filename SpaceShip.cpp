#include "pch.h"
#include "SpaceShip.h"

SpaceShip::SpaceShip()
	: m_Tag{ "SpaceShip"},
	  m_TestForCulling{ false }
{
	this->m_model = ModelFactory::Get().GetModel(std::string("models/spaceship_basic.obj"));
	//this->m_model = ModelFactory::Get().GetModel(std::string("models/cubemetal.obj"));
	this->m_wMatrix = {
		0.03f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.03f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.03f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	this->m_center = { 0.0f, 1000.0f, -10000.0f };
	this->m_mass = 10000.0f;
	m_scale = 0.5f;
	m_yaw = (float)M_PI;
	m_pitchTilt = 0.0f;
	m_rollTilt = 0.0f;
	m_velocity = { 1.0f, 1.0f, 1.0f };

	// Upgrades
	std::string folder = "models/";
	std::vector<std::string> upgradeModels = {
		"spaceship_afterburner.obj",
		"spaceship_antenna.obj",
		"spaceship_cargo.obj",
		"spaceship_cold.obj",
		"spaceship_fuelcells.obj",
		"spaceship_livingquarters.obj",
		"spaceship_shield.obj",
		"spaceship_warm.obj",
		"spaceship_warpdrive.obj"
	};
	for (std::string file : upgradeModels)
	{
		m_upgrades.push_back(new SpaceShipUpgrade(folder + file));
	}
}

SpaceShip::~SpaceShip()
{
	for (GameObject* upgrade : m_upgrades)
	{
		if (upgrade) delete upgrade;
	}
}

GameObject* SpaceShip::update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Spaceship");
	ImGui::Text("Center  : (%f, %f, %f)", m_center.x, m_center.y, m_center.z);
	ImGui::Text("Velocity: (%f, %f, %f)", m_velocity.x, m_velocity.y, m_velocity.z);
	ImGui::Text("Speed	: %f", length(m_velocity));
	//ImGui::DragFloat("Mass", &m_mass, 500.0f);
	ImGui::End();
#endif

	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&this->m_upVector);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_scale, m_scale, m_scale);
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(m_pitch + m_pitchTilt);
	DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(m_yaw);
	DirectX::XMVECTOR forward{ m_forwardVector.x, m_forwardVector.y, m_forwardVector.z };
	DirectX::XMMATRIX roll = DirectX::XMMatrixRotationAxis(forward, m_roll + m_rollTilt);
	DirectX::XMMATRIX rot = rotX * rotY * roll;

	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(this->m_center.x, this->m_center.y, this->m_center.z);
	DirectX::XMMATRIX final = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, DirectX::XMMatrixTranspose(final));

	//Update the matrixBuffer.
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	DirectX::XMFLOAT4X4 wvpMatrix;
	DirectX::XMMATRIX vMatrix = DirectX::XMLoadFloat4x4(&VMatrix);
	DirectX::XMMATRIX pMatrix = DirectX::XMLoadFloat4x4(&PMatrix);

	DirectX::XMStoreFloat4x4(&wvpMatrix, DirectX::XMMatrixTranspose(final * vMatrix * pMatrix));

	deviceContext->Map(this->m_model->getMatrixBuffer().Get(),
			           0,
		               D3D11_MAP_WRITE_DISCARD,
		               0,
		               &mappedSubresource);

	memcpy(mappedSubresource.pData, &m_wMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(((char*)mappedSubresource.pData) + sizeof(DirectX::XMFLOAT4X4), &wvpMatrix, sizeof(DirectX::XMFLOAT4X4));

	deviceContext->Unmap(this->m_model->getMatrixBuffer().Get(), 0);
	return nullptr;
}

void SpaceShip::AddRotation(float yaw, float pitch)
{
	float alpha = 0.1f;

	m_yaw += yaw;
	if (m_yaw >= 2 * (float)M_PI) m_yaw -= 2 * (float)M_PI;
	else if (m_yaw <= -2 * (float)M_PI) m_yaw += 2 * (float)M_PI;

	m_pitch += pitch;
	if (m_pitch > (float)M_PI_2 - alpha) m_pitch = (float)M_PI_2 - alpha;
	else if (m_pitch < -(float)M_PI_2 + alpha) m_pitch = -(float)M_PI_2 + alpha;
}

void SpaceShip::SetTilt(float pitchLerp, float rollLerp)
{
	m_pitchTilt = pitchLerp * (float)M_PI / 8.0f;
	m_rollTilt = rollLerp * (float)M_PI_4;
}

void SpaceShip::SetForwardVector(DirectX::XMFLOAT3 forwardVector)
{
	m_forwardVector = forwardVector;
}

float SpaceShip::GetTopSpeed()
{
	return m_topSpeed;
}

DirectX::XMFLOAT3 SpaceShip::GetVelocity()
{
	return m_velocity;
}

const bool SpaceShip::IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept
{
	return false;
}

DirectX::XMFLOAT3 SpaceShip::getCenter() {
	return this->m_center;
}

void SpaceShip::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	for (auto tex : m_model->GetTextures())
	{
		if (tex) tex->Bind(deviceContext);
	}
	deviceContext->IASetVertexBuffers(0u,
									  1u,
									  this->m_model->getVertexBuffer().GetAddressOf(),
									  &this->m_model->getStride(),
									  &this->m_model->getOffset());
	
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}

void SpaceShip::BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->IASetVertexBuffers(0u,
									  1u,
									  this->m_model->getVertexBuffer().GetAddressOf(),
									  &this->m_model->getStride(),
									  &this->m_model->getOffset());
	pDeviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
}


const std::string& SpaceShip::GetTag() const noexcept
{
	return m_Tag;
}

const bool& SpaceShip::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}

std::vector<GameObject*>& SpaceShip::GetUpgrades()
{
	return m_upgrades;
}