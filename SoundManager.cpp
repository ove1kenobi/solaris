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

void SoundManager::OnEvent(IEvent& event) noexcept
{
}
