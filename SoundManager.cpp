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

bool SoundManager::Initialize(float musicVolume, float sfxVolumn)
{
	m_musicVolume = musicVolume;
	m_sfxVolume = sfxVolumn;

	EventBuss::Get().AddListener(this, EventType::PlaySoundEvent, EventType::StopLoopingSoundEvent, EventType::SetMusicEvent);
	
	m_songs.push_back("Sounds/SpaceAmbience.ogg");
	
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("Sounds/thrusters2.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/ship_destroyed.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/collision_asteroid.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/trusters2_end.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/stabilizers.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	//	if (!buffer.loadFromFile("Sounds/beep.wav"))
	//		return false;
	//	else
	//		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/event_screen.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/hover.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	if (!buffer.loadFromFile("Sounds/pay_upgrade.wav"))
		return false;
	else
		m_sounds.push_back(buffer);

	for (unsigned int i = 0; i < m_sounds.size(); i++) {
		m_loopingSounds.push_back(sf::Sound(m_sounds[i]));
		m_loopingSounds[i].setLoop(true);
	}

	m_music.openFromFile(m_songs[(int)MusicID::SpaceAmbience]);
	m_music.setVolume(m_musicVolume);
	m_music.setLoop(true);
	m_music.play();

	return true;
}

void SoundManager::Update()
{
#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Sound volume");
	ImGui::DragFloat("Music volume", &m_musicVolume, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("SFX volume", &m_sfxVolume, 1.0f, 0.0f, 100.0f);
	ImGui::End();
#endif

	m_music.setVolume(m_musicVolume);

	// Remove sound that have finished playing
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
			SoundID soundID = static_cast<PlaySoundEvent*>(&event)->GetSoundID();
			bool loopSound = static_cast<PlaySoundEvent*>(&event)->GetLoopSound();
			float pitch = static_cast<PlaySoundEvent*>(&event)->GetPitch();

			if (loopSound) {
				if (m_loopingSounds[(int)soundID].getStatus() != sf::Sound::Status::Playing) {
					m_loopingSounds[(int)soundID].setVolume(m_sfxVolume);
					m_loopingSounds[(int)soundID].setPitch(pitch);
					m_loopingSounds[(int)soundID].play();
				}
			}
			else {
				m_playingSounds.push(sf::Sound(m_sounds[(int)soundID]));
				m_playingSounds.back().setVolume(m_sfxVolume);
				m_playingSounds.back().setPitch(pitch);
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
			if ((int)musicID < 0) {
				m_music.stop();
			}
			else {
				m_music.openFromFile(m_songs[(int)musicID]);
				m_music.setLoop(true);
				m_music.play();
			}

			break;
		}
	}
}
