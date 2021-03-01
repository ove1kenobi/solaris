#pragma once
#include "IEvent.h"

enum class SoundID {
	Shield = 0,
	Thrusters
};

enum class MusicID {
	Silence = 0,
	SpaceAmbience
};

class PlaySoundEvent : public IEvent
{
private:
	SoundID m_soundID;
	bool m_loopSound;
public:
	PlaySoundEvent(SoundID soundID, bool loopSound)
	{
		m_soundID = soundID;
		m_loopSound = loopSound;
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
	[[nodiscard]] const SoundID GetSoundID() const noexcept
	{
		return m_soundID;
	}
	[[nodiscard]] const bool GetLoopSound() const noexcept
	{
		return m_loopSound;
	}
};

class StopLoopingSoundEvent : public IEvent
{
private:
	SoundID m_soundID;
public:
	StopLoopingSoundEvent(SoundID soundID)
	{
		m_soundID = soundID;
	}
	virtual ~StopLoopingSoundEvent() noexcept = default;

	[[nodiscard]] const EventType GetEventType() const noexcept
	{
		return EventType::StopLoopingSoundEvent;
	}
	[[nodiscard]] const std::string GetDebugName() const noexcept
	{
		return "StopLoopingSoundEvent";
	}
	[[nodiscard]] const SoundID GetSoundID() const noexcept
	{
		return m_soundID;
	}
};

class SetMusicEvent : public IEvent
{
private:
	MusicID m_musicID;
public:
	SetMusicEvent(MusicID musicID)
	{
		m_musicID = musicID;
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
	[[nodiscard]] const MusicID GetMusicID() const noexcept
	{
		return m_musicID;
	}

};