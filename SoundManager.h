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

	//sf::Sound test1, test2;

	float m_musicVolume;
	float m_sfxVolume;
public:
	SoundManager();
	~SoundManager();

	bool Initialize();
	void Update();

	void OnEvent(IEvent& event) noexcept;
};

