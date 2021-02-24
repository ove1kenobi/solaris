#include "pch.h"
#include "Render2D.h"

Render2D::Render2D() noexcept {
	//Make render2D able to UI handle events (for now, only keyboard ones)
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent);

	//Set start UI and load them all into an vector
	m_CurrentUI = TypesUI::PlanetInteraction;
	m_Modules.push_back(new PlanetInteractionUI());
	m_Modules.push_back(new HeadsUpDisplayUI());

	m_Render = false;
}

Render2D::~Render2D() {
	for (unsigned int i = 0; i < m_Modules.size(); i++) {
		delete m_Modules.at(i);
	}
}

const bool Render2D::Initialize() noexcept {
	for (unsigned int i = 0; i < m_Modules.size(); i++) {
		if (!m_Modules.at(i)->Initialize()) {
			return false;
		};
	}
	return true;
}

void Render2D::RenderUI() {
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
				if (virKey == 'E') {
					m_CurrentUI = TypesUI::PlanetInteraction;
					if (m_Render) {
						m_Render = false;
					}
					else {
						m_Render = true;
					}
				}
				if (virKey == 'R') {
					m_CurrentUI = TypesUI::HUD;
					if (m_Render) {
						m_Render = false;
					}
					else {
						m_Render = true;
					}
				}
			}
		}
			break;
		default:
			break;
	}
}