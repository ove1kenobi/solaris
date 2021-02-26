#include "pch.h"
#include "SpaceShip.h"

SpaceShip::SpaceShip()
{
	this->m_model = ModelFactory::Get().GetModel(std::string("models/SciFi_Fighter_AK5.obj"));
	//this->m_model = ModelFactory::Get().GetModel(std::string("models/cubemetal.obj"));
	this->m_wMatrix = {
		0.03f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.03f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.03f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	this->m_center = { 0.0f, 1000.0f, -10000.0f };
	this->m_mass = 10000.0f;
	m_yaw = (float)M_PI;
	m_pitchTilt = 0.0f;
	m_rollTilt = 0.0f;
	m_velocity = { 1.0f, 1.0f, 1.0f };
}

GameObject* SpaceShip::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Spaceship");
	ImGui::Text("Center  : (%f, %f, %f)", m_center.x, m_center.y, m_center.z);
	ImGui::Text("Velocity: (%f, %f, %f)", m_velocity.x, m_velocity.y, m_velocity.z);
	ImGui::DragFloat("Mass", &m_mass, 500.0f);
	ImGui::End();
#endif

	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&this->m_upVector);
	//100 times smaller. TODO: make variable?
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(m_pitch + m_pitchTilt);
	DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(m_yaw);
	DirectX::XMVECTOR forward{ m_forwardVector.x, m_forwardVector.y, m_forwardVector.z };
	DirectX::XMMATRIX roll = DirectX::XMMatrixRotationAxis(forward, m_roll + m_rollTilt);
	DirectX::XMMATRIX rot = rotX * rotY * roll;

	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(this->m_center.x, this->m_center.y, this->m_center.z);
	DirectX::XMMATRIX final = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);

	//Update the matrixBuffer.
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	DirectX::XMMATRIX WMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&this->m_wMatrix));
	VMatrix = DirectX::XMMatrixTranspose(VMatrix);
	PMatrix = DirectX::XMMatrixTranspose(PMatrix);

	deviceContext->Map(this->m_model->getMatrixBuffer().Get(),
			           0,
		               D3D11_MAP_WRITE_DISCARD,
		               0,
		               &mappedSubresource);

	ModelFactory::MatrixBuffer* data = (ModelFactory::MatrixBuffer*)mappedSubresource.pData;

	data->WMatrix = WMatrix;
	data->VMatrix = VMatrix;
	data->PMatrix = PMatrix;

	deviceContext->Unmap(this->m_model->getMatrixBuffer().Get(), 0);
	return nullptr;
}

void SpaceShip::Move(float step)
{
	DirectX::XMFLOAT3 pos;

	pos.x = step * m_forwardVector.x;
	pos.y = step * m_forwardVector.y;
	pos.z = step * m_forwardVector.z;

	m_center.x += pos.x;
	m_center.y += pos.y;
	m_center.z += pos.z;
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

void SpaceShip::SetForwardVector(DirectX::XMFLOAT3 cameraPos)
{
	// Create a vector from the camera to the ship 
	m_forwardVector.x = m_center.x - cameraPos.x;
	m_forwardVector.y = m_center.y - cameraPos.y;
	m_forwardVector.z = m_center.z - cameraPos.z;
	
	float length = sqrtf(powf(m_forwardVector.x, 2) + powf(m_forwardVector.y, 2) + powf(m_forwardVector.z, 2));

	// Normalize the vector
	m_forwardVector.x /= length;
	m_forwardVector.y /= length;
	m_forwardVector.z /= length;
}

const bool SpaceShip::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
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