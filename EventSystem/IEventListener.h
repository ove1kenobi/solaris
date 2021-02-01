#pragma once
class IEventListener
{
private:

public:
	IEventListener() noexcept = default;
	virtual ~IEventListener() noexcept = default;
	virtual void OnEvent(const IEvent& event) noexcept = 0;
};
