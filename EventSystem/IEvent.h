#pragma once
#include <string>
#include "EventType.h"
class IEvent
{
private:
	bool m_Handled;
public:
	IEvent() noexcept;
	virtual ~IEvent() noexcept = default;
	virtual [[nodiscard]] const EventType GetEventType() const noexcept = 0;
	virtual [[nodiscard]] const std::string GetDebugName() const noexcept = 0;
	[[nodiscard]] const bool& IsHandled() const noexcept;
	void SetAsHandled() noexcept;
};