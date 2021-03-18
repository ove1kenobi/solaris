#pragma once
#include "EventSystem\IEventListener.h"
#include "EventSystem\EventPublisher.h"
#include "EventSystem/EventBuss.h"
#include "EventSystem/SoundEvents.h"
#include <SFML/Audio.hpp>

class SoundManager : public IEventListener, public EventPublisher
{
private:
	// Currently playing music
	sf::Music m_music;
	// List of music that can be streamed
	std::vector<std::string> m_songs;
	// List of sound that can be played
	std::vector<sf::SoundBuffer> m_sounds;
	// Looping sounds
	std::vector<sf::Sound> m_loopingSounds;
	//  Currently playing sounds
	std::queue<sf::Sound> m_playingSounds;

	float m_musicVolume;
	float m_sfxVolume;
public:
	SoundManager();
	virtual ~SoundManager();

	bool Initialize(float musicVolume = 100.0f, float sfxVolume = 100.0f);
	void Update();

	void OnEvent(IEvent& event) noexcept;
};

