#pragma once
#include "EventSystem\IEventListener.h"
#include <SFML/Audio.hpp>
#include <queue>
#include <vector>

class SoundManager : public IEventListener
{
private:
	sf::Music m_music;
	std::vector<sf::SoundBuffer> m_sounds;
	std::queue<sf::Sound> m_playingSounds;
	
	float m_musicVolume;
	float m_sfxVolume;
public:
	SoundManager();
	~SoundManager();

	void OnEvent(IEvent& event) noexcept;
};

