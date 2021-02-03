#pragma once
#include "IEvent.h"
class IEventListener
{
private:

public:
	IEventListener() noexcept = default;
	virtual ~IEventListener() noexcept = default;
	virtual void OnEvent(IEvent& event) noexcept = 0;
};
