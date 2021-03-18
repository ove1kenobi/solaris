#pragma once
#include "IEvent.h"

enum class SoundID {
	Thrusters = 0,
	ShipExplosion,
	AsteroidCollision,
	ThrustersEnd,
	Stabilizers,
	EventScreen,
	Hover,
	PayUpgrade,
	Wrong,
	Warning,
	Click,
	Scan,
	WarpDrive,
	ActivateStabilizers
};

enum class MusicID {
	Silence = -1,
	SpaceAmbience = 0
};

class PlaySoundEvent : public IEvent
{
private:
	SoundID m_soundID;
	bool m_loopSound;
	float m_pitch;
public:
	PlaySoundEvent(SoundID soundID, bool loopSound, float pitch = 1.0f)
	{
		m_soundID = soundID;
		m_loopSound = loopSound;
		m_pitch = pitch;
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
	[[nodiscard]] const float GetPitch() const noexcept
	{
		return m_pitch;
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