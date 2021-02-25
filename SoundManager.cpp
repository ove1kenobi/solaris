#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	m_musicVolume = 50.0f;
	m_sfxVolume = 50.0f;
}

SoundManager::~SoundManager()
{

}

bool SoundManager::Initialize()
{
	EventBuss::Get().AddListener(this, EventType::PlaySoundEvent, EventType::SetMusicEvent);

	sf::SoundBuffer buffer;
	
	if (!buffer.loadFromFile("Sounds/forceField_001.ogg"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/thrusterFire_003.ogg"))
		return false;
	else
		m_sounds.push_back(buffer);
	
	return true;
}

void SoundManager::Update()
{
	while (!m_playingSounds.empty() && m_playingSounds.front().getStatus() == sf::Sound::Status::Stopped) {
		m_playingSounds.pop();
	}
}

void SoundManager::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
		case EventType::PlaySoundEvent:
		{
			sf::Sound sound(m_sounds[0]);
			sound.play();
			m_playingSounds.push(sound);
		}
		case EventType::SetMusicEvent:
		{

		}
	}
}
