#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	m_musicVolume = 100.0f;
	m_sfxVolume = 100.0f;
}

SoundManager::~SoundManager()
{

}

bool SoundManager::Initialize()
{
	EventBuss::Get().AddListener(this, EventType::PlaySoundEvent, EventType::StopLoopingSoundEvent, EventType::SetMusicEvent);

	sf::SoundBuffer buffer;
	
	if (!buffer.loadFromFile("Sounds/forceField_001.ogg"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/thrusterFire_003.ogg"))
		return false;
	else
		m_sounds.push_back(buffer);

	for (unsigned int i = 0; i < m_sounds.size(); i++) {
		m_loopingSounds.push_back(sf::Sound(m_sounds[i]));
		m_loopingSounds[i].setLoop(true);
	}

	return true;
}

void SoundManager::Update()
{
	while (!m_playingSounds.empty() && m_playingSounds.front().getStatus() == sf::Sound::Status::Stopped) {
		//delete m_playingSounds.front();
		m_playingSounds.pop();
	}
}

void SoundManager::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
		case EventType::PlaySoundEvent:
		{
			SoundID soundID = static_cast<PlaySoundEvent*>(&event)->GetSoundID();
			bool loopSound = static_cast<PlaySoundEvent*>(&event)->GetLoopSound();

			if (loopSound) {
				if (m_loopingSounds[(int)soundID].getStatus() != sf::Sound::Status::Playing) {
					m_loopingSounds[(int)soundID].play();
				}
			}
			else {
				m_playingSounds.push(sf::Sound(m_sounds[(int)soundID]));
				m_playingSounds.back().play();
			}
			break;
		}
		case EventType::StopLoopingSoundEvent:
		{
			SoundID soundID = static_cast<StopLoopingSoundEvent*>(&event)->GetSoundID();
			m_loopingSounds[(int)soundID].stop();
			break;
		}
		case EventType::SetMusicEvent:
		{
			MusicID musicID = static_cast<SetMusicEvent*>(&event)->GetMusicID();
			break;
		}
	}
}
