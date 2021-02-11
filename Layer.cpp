#include "pch.h"
#include "Layer.h"

Layer::Layer() noexcept
	: m_IsActiveLayer{ false }
{

}

const bool& Layer::IsActiveLayer() const noexcept
{
	return m_IsActiveLayer;
}

void Layer::ToggleActiveLayer() noexcept
{
	m_IsActiveLayer = !m_IsActiveLayer;
}