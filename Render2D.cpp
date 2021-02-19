#include "pch.h"
#include "Render2D.h"

Render2D::Render2D() noexcept {
	m_TestUI = new PlanetInteractionUI();
	m_RenderPlanetInteraction = false;
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent);
}

Render2D::~Render2D() {
	delete m_TestUI;
}

const bool Render2D::Initialize() noexcept {
	if (!m_TestUI->Initialize()) {
		return false;
	}
	return true;
}

void Render2D::RenderUI() {
	if (m_RenderPlanetInteraction) {
		m_TestUI->BeginFrame();

		m_TestUI->Render();

		m_TestUI->EndFrame();
	}
}

void Render2D::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::KeyboardEvent:
		{
			KeyState state = static_cast<KeyboardEvent*>(&event)->GetKeyState();
			int virKey = static_cast<KeyboardEvent*>(&event)->GetVirtualKeyCode();

			if (state == KeyState::KeyPress) {
				if (virKey == 'E') {
					if (m_RenderPlanetInteraction) {
						m_RenderPlanetInteraction = false;
					}
					else {
						m_RenderPlanetInteraction = true;
					}
				}
			}
		}
			break;
		default:
			break;
	}
}