#pragma once
#include "EventSystem\IEventListener.h"
class Layer : public IEventListener
{
private:
	bool m_IsActiveLayer;
public:
	Layer() noexcept;
	virtual ~Layer() noexcept = default;
	virtual void Update() noexcept {};
	virtual void OnEvent(IEvent& event) noexcept = 0;
	virtual [[nodiscard]] const std::string GetDebugName() const noexcept = 0;
	[[nodiscard]] const bool& IsActiveLayer() const noexcept;
	void ToggleActiveLayer() noexcept;
};