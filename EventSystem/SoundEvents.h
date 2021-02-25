#pragma once
#include "IEvent.h"

class PlaySoundEvent : public IEvent
{
private:

public:
	PlaySoundEvent()
	{
	}
	virtual ~PlaySoundEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::PlaySoundEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "PlaySoundEvent";
	}

};

class SetMusicEvent : public IEvent
{
private:

public:
	SetMusicEvent()
	{
	}
	virtual ~SetMusicEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::SetMusicEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "SetMusicEvent";
	}

};