#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack() noexcept
	: m_EndOfNormalLayers{ m_Layers.begin() }
{
}

void LayerStack::Push(Layer* pLayer) noexcept
{
	m_EndOfNormalLayers = m_Layers.emplace(m_EndOfNormalLayers,pLayer);
}

void LayerStack::PushOverlay(Layer* pLayer) noexcept
{
	m_Layers.emplace_back(pLayer);
}

void LayerStack::Update() noexcept
{
	for (int i = static_cast<int>(m_Layers.size()) - 1; i > -1; i--)
	{
		m_Layers[i]->Update();
	}
}