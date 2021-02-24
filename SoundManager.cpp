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
{/*
	test1.setBuffer(m_sounds[0]);
	test2.setBuffer(m_sounds[1]);

	test1.play();
	test2.play();*/
}

void SoundManager::OnEvent(IEvent& event) noexcept
{
}
