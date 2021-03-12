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
	  m_PlanetInteractionUIOpen{ false }
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

	m_Render = false;
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

void Render2D::RenderUI() {
	//For now always render HUD
	m_Modules.at(static_cast<int>(TypesUI::HUD))->BeginFrame();
	m_Modules.at(static_cast<int>(TypesUI::HUD))->Render();
	m_Modules.at(static_cast<int>(TypesUI::HUD))->EndFrame();

	if (m_pPlayerInfo->distanceToClosestPlanet <= DISTANCE_THRESHOLD
		&& m_pPlayerInfo->closestPlanet->IsVisited() == false 
		&& !m_Render) {
		m_Modules.at(static_cast<int>(TypesUI::PressInteract))->BeginFrame();
		m_Modules.at(static_cast<int>(TypesUI::PressInteract))->Render();
		m_Modules.at(static_cast<int>(TypesUI::PressInteract))->EndFrame();
	}

	if (m_Render) {
		m_Modules.at(static_cast<int>(m_CurrentUI))->BeginFrame();
		m_Modules.at(static_cast<int>(m_CurrentUI))->Render();
		m_Modules.at(static_cast<int>(m_CurrentUI))->EndFrame();
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
					if (m_pPlayerInfo->distanceToClosestPlanet <= DISTANCE_THRESHOLD
						&& m_pPlayerInfo->closestPlanet->IsVisited() == false)
					{
						ToggleControlsEvent controlsEvent;
						EventBuss::Get().Delegate(controlsEvent);
						m_CurrentUI = TypesUI::PlanetInteraction;
						ToggleTetheredEvent TTEvent;
						EventBuss::Get().Delegate(TTEvent);
						if (m_Render) {
							m_Render = false;
							m_pPlayerInfo->closestPlanet->MarkAsVisited();
						}
						else {
							m_Render = true;
						}
					}
				}
				if (virKey == 'R') {
					m_CurrentUI = TypesUI::MainMenu;
					if (m_Render) {
						m_Render = false;
					}
					else {
						m_Render = true;
					}
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
		default:
			break;
	}
}