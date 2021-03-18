#include "pch.h"
#include "Render2D.h"

bool Render2D::AddFonts() {
	//Load in fonts which the UI will use
	if (AddFontResource(this->GetFontFilePath(L"Netron-p7gq1.otf").c_str()) == 0) {
		return false;
	}
	if (AddFontResource(this->GetFontFilePath(L"AwareBold-qZo3x.ttf").c_str()) == 0) {
		return false;
	}
	if (AddFontResource(this->GetFontFilePath(L"Tenika400Regular-Rpyql.ttf").c_str()) == 0) {
		return false;
	}
	if (AddFontResource(this->GetFontFilePath(L"Neoterique-Y08L.ttf").c_str()) == 0) {
		return false;
	}
	if (AddFontResource(this->GetFontFilePath(L"NeoteriqueItalic-rAVK.ttf").c_str()) == 0) {
		return false;
	}
	return true;
}

Render2D::Render2D() noexcept 
	: m_pPlayerInfo{ nullptr },
	  m_PlanetInteraction{ false },
	  m_UpgradeScreen{ false },
	  m_InGame{ false },
	  m_CanWin{false},
	  m_Credits{ false },
	  m_Controls{ false },
	  m_DisplayText{ false },
	  m_Paused{ false }
{
	//Make render2D able to UI handle events (for now, only keyboard ones)
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent, EventType::DelegatePlayerInfoEvent);
	EventBuss::Get().AddListener(this, EventType::TogglePressToWinGame);
	EventBuss::Get().AddListener(this, EventType::ToggleControls);
	EventBuss::Get().AddListener(this, EventType::ToggleCredits);
	EventBuss::Get().AddListener(this, EventType::DisplayEndgameText);
	EventBuss::Get().AddListener(this, EventType::TogglePauseGame);

	if (AddFonts()) {
		//Set start UI and load them all into an vector
		m_Modules.push_back(new PlanetInteractionUI());
		m_Modules.push_back(new HeadsUpDisplayUI());
		m_Modules.push_back(new MenuUI());
		m_Modules.push_back(new PressInteractUI());
		m_Modules.push_back(new CrosshairUI());
		m_Modules.push_back(new UpgradeScreenUI());
		m_Modules.push_back(new ControlsUI());
		m_Modules.push_back(new CreditsUI());
		m_Modules.push_back(new PressWinUI());
		m_Modules.push_back(new EndgameUI());
		m_Modules.push_back(new PauseUI());

		m_Modules.at(static_cast<int>(TypesUI::MainMenu))->m_pOnScreen = true;
	}
}

Render2D::~Render2D() {
	for (unsigned int i = 0; i < m_Modules.size(); i++) {
		delete m_Modules.at(i);
	}
	//Remove fonts used
	RemoveFontResource(this->GetFontFilePath(L"AwareBold-qZo3x.ttf").c_str());
	RemoveFontResource(this->GetFontFilePath(L"Tenika400Regular-Rpyql.ttf").c_str());
	RemoveFontResource(this->GetFontFilePath(L"NeoteriqueItalic-rAVK.ttf").c_str());
	RemoveFontResource(this->GetFontFilePath(L"Neoterique-Y08L.ttf").c_str());
	RemoveFontResource(this->GetFontFilePath(L"Netron-p7gq1.otf").c_str());

	EventBuss::Get().RemoveListener(this, EventType::KeyboardEvent);
	EventBuss::Get().RemoveListener(this, EventType::DelegatePlayerInfoEvent);
	EventBuss::Get().RemoveListener(this, EventType::TogglePressToWinGame);
	EventBuss::Get().RemoveListener(this, EventType::ToggleControls);
	EventBuss::Get().RemoveListener(this, EventType::ToggleCredits);
	EventBuss::Get().RemoveListener(this, EventType::DisplayEndgameText);
	EventBuss::Get().RemoveListener(this, EventType::TogglePauseGame);
}

const bool Render2D::Initialize() noexcept {
	for (unsigned int i = 0; i < m_Modules.size(); i++) {
		if (!m_Modules.at(i)->Initialize()) {
			return false;
		};
	}
	return true;
}

std::wstring Render2D::GetFontFilePath(std::wstring fontFile) {
	std::wstring FilePath;

	//Get current directory
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	FilePath.append(NPath);

	//Get font file
	FilePath.append(L"\\UI\\Fonts\\");
	FilePath.append(fontFile);

	return FilePath;
}

void Render2D::RenderUI() {
	if (m_InGame) {
		//Always render HUD while in game
		m_Modules.at(static_cast<int>(TypesUI::HUD))->Render();

		//If close to planet render press interact
		if (m_pPlayerInfo->distanceToClosestPlanet <= DISTANCE_THRESHOLD
			&& m_pPlayerInfo->closestPlanet->IsVisited() == false
			&& !m_PlanetInteraction) {
			m_Modules.at(static_cast<int>(TypesUI::PressInteract))->Render();
		}

		if (m_CanWin && !m_PlanetInteraction && !m_UpgradeScreen) {
			m_Modules.at(static_cast<int>(TypesUI::PressWin))->Render();
		}

		//If player was able to interact with planet, render it
		if (m_PlanetInteraction) {
			m_Modules.at(static_cast<int>(TypesUI::PlanetInteraction))->Render();
		}
		//If player wants to upgrade, render it
		if (m_UpgradeScreen) {
			m_Modules.at(static_cast<int>(TypesUI::UpgradeScreen))->Render();
		}

		if (m_DisplayText) {
			m_Modules.at(static_cast<int>(TypesUI::Endgame))->Render();
		}

		if (m_Paused && !m_DisplayText) {
			m_Modules.at(static_cast<int>(TypesUI::Pause))->Render();
		}
	}
	//If not in game then render main menu
	else {
		ClearBackBufferEvent cbbEvent;
		EventBuss::Get().Delegate(cbbEvent);
		m_Modules.at(static_cast<int>(TypesUI::MainMenu))->Render();

		if (m_Controls) {
			m_Modules.at(static_cast<int>(TypesUI::Controls))->Render();
		}

		if (m_Credits) {
			m_Modules.at(static_cast<int>(TypesUI::Credits))->Render();
		}
	}
	//And finally always render crosshair on top of everything
	m_Modules.at(static_cast<int>(TypesUI::Crosshair))->Render();
}

void Render2D::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::KeyboardEvent:
		{
			KeyState state = static_cast<KeyboardEvent*>(&event)->GetKeyState();
			int virKey = static_cast<KeyboardEvent*>(&event)->GetVirtualKeyCode();

			//An event has to have been selected before the player is allowed to press E again,
			//Either during a previous run, or it starts on true by default
			//As it get's set to false as soon as PlanetInteractionUI fetches random events
			if (state == KeyState::KeyPress) {
				/*
				Requriements to enter and exit planetInteraction:
				Player press "E"
				They are close enough to the planet
				They have an event picked (to prevent them from exiting)
				*/
				if (virKey == 'E' && m_InGame)
				{
					if (m_pPlayerInfo)
					{
						if (m_pPlayerInfo->distanceToClosestPlanet <= DISTANCE_THRESHOLD
							&& m_pPlayerInfo->closestPlanet->IsVisited() == false &&
							static_cast<PlanetInteractionUI*>(m_Modules[(int)TypesUI::PlanetInteraction])->m_gameEventSelected == true)
						{
							//Stop thruster sound if interacting with a planet
							StopLoopingSoundEvent thrusterSound(SoundID::Thrusters);
							EventBuss::Get().Delegate(thrusterSound);

							//Planet interaction events
							ToggleControlsEvent controlsEvent;
							EventBuss::Get().Delegate(controlsEvent);
							ToggleTetheredEvent TTEvent;
							EventBuss::Get().Delegate(TTEvent);
							PlaySoundEvent playSoundEvent(SoundID::EventScreen, false);
							EventBuss::Get().Delegate(playSoundEvent);

							//Render planet interaction
							if (m_PlanetInteraction) {
								m_PlanetInteraction = false;
								m_pPlayerInfo->closestPlanet->MarkAsVisited();
							}
							else {
								m_PlanetInteraction = true;
								GameEvent gameEvents[3];
								gameEventManager.RequestGameEvents(gameEvents, m_pPlayerInfo->closestPlanet->GetPlanetType());
								static_cast<PlanetInteractionUI*>(m_Modules[(int)TypesUI::PlanetInteraction])->SetPlanetFlavourText(m_pPlayerInfo->planetInteractedWith->GetPlanetFlavourText());
								static_cast<PlanetInteractionUI*>(m_Modules[(int)TypesUI::PlanetInteraction])->SetGameEvents(gameEvents);
							}
							m_Modules.at(static_cast<int>(TypesUI::PlanetInteraction))->m_pOnScreen = m_PlanetInteraction;
						}
					}
				}
				//If player goes in the upgrade screen we also do not want things to update
				if (virKey == 'U' && !m_PlanetInteraction && m_InGame) {
					//Stop thruster sound if in upgrade menu.
					StopLoopingSoundEvent thrusterSound(SoundID::Thrusters);
					EventBuss::Get().Delegate(thrusterSound);

					ToggleControlsEvent controlsEvent;
					EventBuss::Get().Delegate(controlsEvent);
					PlaySoundEvent playSoundEvent(SoundID::EventScreen, false);
					EventBuss::Get().Delegate(playSoundEvent);
					m_UpgradeScreen = !m_UpgradeScreen;
					m_Modules.at(static_cast<int>(TypesUI::UpgradeScreen))->m_pOnScreen = m_UpgradeScreen;
				}

				if (virKey == VK_ESCAPE && m_InGame) {
					//Stop thruster sound if the pause menu gets opened.
					StopLoopingSoundEvent thrusterSound(SoundID::Thrusters);
					EventBuss::Get().Delegate(thrusterSound);

					ToggleControlsEvent controlsEvent;
					EventBuss::Get().Delegate(controlsEvent);
					PlaySoundEvent playSoundEvent(SoundID::EventScreen, false);
					EventBuss::Get().Delegate(playSoundEvent);
					m_Paused = !m_Paused;
					m_Modules.at(static_cast<int>(TypesUI::Pause))->m_pOnScreen = m_Paused;
				}

				if (virKey == 'R' && m_CanWin) {
					m_CanWin = false;
				}
			}
			break;
		}
		case EventType::DelegatePlayerInfoEvent:
		{
			DelegatePlayerInfoEvent& derivedEvent = static_cast<DelegatePlayerInfoEvent&>(event);
			m_pPlayerInfo = derivedEvent.GetPlayerInfo();
			break;
		}
		case EventType::TogglePressToWinGame:
		{
			m_CanWin = true;
			break;
		}
		case EventType::ToggleCredits:
		{
			//main menu is false once in here
			m_Modules.at(static_cast<int>(TypesUI::MainMenu))->m_pOnScreen = m_Credits;
			m_Credits = !m_Credits;
			m_Modules.at(static_cast<int>(TypesUI::Credits))->m_pOnScreen = m_Credits;
			break;
		}
		case EventType::ToggleControls:
		{
			//main menu is false once in here
			m_Modules.at(static_cast<int>(TypesUI::MainMenu))->m_pOnScreen = m_Controls;
			m_Controls = !m_Controls;
			m_Modules.at(static_cast<int>(TypesUI::Controls))->m_pOnScreen = m_Controls;
			break;
		}
		case EventType::TogglePauseGame:
		{
			m_Paused = !m_Paused;
			ToggleControlsEvent controlsEvent;
			EventBuss::Get().Delegate(controlsEvent);
			PlaySoundEvent playSoundEvent(SoundID::EventScreen, false);
			EventBuss::Get().Delegate(playSoundEvent);
			m_Modules.at(static_cast<int>(TypesUI::Pause))->m_pOnScreen = m_Paused;
			break;
		}
		case EventType::DisplayEndgameText:
		{
			m_DisplayText = true;
			std::wstring endgameText = static_cast<DisplayEndgameText*>(&event)->GetEndgameText();
			static_cast<EndgameUI*>(m_Modules[(int)TypesUI::Endgame])->SetText(endgameText);
			break;
		}
		default:
			break;
	}
}

void Render2D::CleanUp() {
	for (auto r : m_Modules)
		r->CleanUp();
}

void Render2D::ToggleMainMenu() {
	m_InGame = !m_InGame;
}
