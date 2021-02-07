#pragma once
#include "EventBuss.h"
class EventPublisher
{
private:

public:
	EventPublisher() noexcept = default;
	virtual ~EventPublisher() noexcept = default;
};