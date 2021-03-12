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
	  m_Render{ false }
{
	//Make render2D able to UI handle events (for now, only keyboard ones)
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent, EventType::DelegatePlayerInfoEvent);

	if (AddFonts()) {
		//Set start UI and load them all into an vector
		m_CurrentUI = TypesUI::PlanetInteraction;
		m_Modules.push_back(new PlanetInteractionUI());
		m_Modules.push_back(new HeadsUpDisplayUI());
		m_Modules.push_back(new MenuUI());
		m_Modules.push_back(new PressInteractUI());
	}

	//Need to be set to false once we have the main menu working correctly
	m_InGame = true;
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

//Here's where I would have put my state machine...
//IF I HAD THE TIME TO MAKE ONE
//(currently we use if statements to figure out what to render when)

//How do we convey to things here not to accept input when they don't need it?
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

		//If player was able to interact with planet, render it
		if (m_PlanetInteraction) {
			m_Modules.at(static_cast<int>(TypesUI::PlanetInteraction))->Render();
		}
	}
	//If not in game then render main menu
	else {
		m_Modules.at(static_cast<int>(TypesUI::MainMenu))->Render();
	}
}

//Will in the future take in UI events to know what to render when
void Render2D::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::KeyboardEvent:
		{
			KeyState state = static_cast<KeyboardEvent*>(&event)->GetKeyState();
			int virKey = static_cast<KeyboardEvent*>(&event)->GetVirtualKeyCode();

			if (state == KeyState::KeyPress) {
				if (virKey == 'E') 
				{
					if (m_pPlayerInfo)
					{
						if (m_pPlayerInfo->distanceToClosestPlanet <= DISTANCE_THRESHOLD
							&& m_pPlayerInfo->closestPlanet->IsVisited() == false)
						{
							//Planet interaction events
							ToggleControlsEvent controlsEvent;
							EventBuss::Get().Delegate(controlsEvent);
							ToggleTetheredEvent TTEvent;
							EventBuss::Get().Delegate(TTEvent);

							//Render planet interaction
							if (m_PlanetInteraction) {
								m_PlanetInteraction = false;
								m_pPlayerInfo->closestPlanet->MarkAsVisited();
							}
							else {
								m_PlanetInteraction = true;
							}
						}
					}
				}
				//Just and example toggle for the menu
				//To do it properly just change the bool of InGame
				if (virKey == 'R') {
					if (m_InGame) {
						m_InGame = false;
					}
					else {
						m_InGame = true;
					}
				}
				//If player goes in the upgrade screen we also do not want things to update
			}
			break;
		}
		case EventType::DelegatePlayerInfoEvent:
		{
			DelegatePlayerInfoEvent& derivedEvent = static_cast<DelegatePlayerInfoEvent&>(event);
			m_pPlayerInfo = derivedEvent.GetPlayerInfo();
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