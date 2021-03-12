#pragma once
#include "Layer.h"
class LayerStack
{
private:
	std::vector<Layer*> m_Layers;
	std::vector<Layer*>::const_iterator m_EndOfNormalLayers;
public:
	LayerStack() noexcept;
	virtual ~LayerStack() noexcept = default;
	void Push(Layer* layer) noexcept;
	void RemoveFirst() noexcept;
	void PushOverlay(Layer* layer) noexcept;
	void Update() noexcept;
};