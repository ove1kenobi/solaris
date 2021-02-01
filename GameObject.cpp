#include "GameObject.h"

GameObject::GameObject() noexcept 
	:	m_velocity{ 0, 0, 0 },
		m_center{ 0, 0, 0 },
		m_wMatrix{
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
		},
		m_mass{ 0 },
		m_pitch{ 0 },
		m_roll{ 0 },
		m_yaw{ 0 }
{

}

void GameObject::getWMatrix(DirectX::XMMATRIX& wMatrix) {
	wMatrix = DirectX::XMLoadFloat4x4(&this->m_wMatrix);
}