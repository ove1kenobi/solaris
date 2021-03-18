#include "..\pch.h"
#include "ControlsUI.h"

ControlsUI::ControlsUI() noexcept {

}

ControlsUI::~ControlsUI() {

}

bool ControlsUI::Initialize() {
	if (!CreateScreen()) {
		return false;
	}
	return true;
}

bool ControlsUI::CreateScreen() {
    return true;
}

bool ControlsUI::UpdateScreen()
{
    return true;
}

bool ControlsUI::UpdateModules() {
	if (!UpdateScreen()) {
		return false;
	}
    return true;
}

void ControlsUI::RenderScreen() {

}

void ControlsUI::RenderBack() {
	//if hover
	if (m_pMouseX > m_pBackTextBox.left && m_pMouseX < m_pBackTextBox.right &&
		m_pMouseY > m_pBackTextBox.top && m_pMouseY < m_pBackTextBox.bottom) {
		//this->UpdateBrush(m_pHighlight, 1.0f);
	}
	else {
		//this->UpdateBrush(m_pWhite, 1.0f);
	}
	m_pRenderTarget2D.Get()->DrawTextW(
		m_pBackText.c_str(),
		(UINT32)m_pBackText.length(),
		m_pButtonFormat.Get(),
		m_pBackTextBox,
		m_pBrush.Get()
	);
}

void ControlsUI::Render() {

}

void ControlsUI::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::DelegateDXEvent:
	{
		UpdateDXHandlers(event);
		break;
	}
	case EventType::DelegateMouseCoordsEvent:
	{
		m_pMouseX = static_cast<DelegateMouseCoordsEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<DelegateMouseCoordsEvent*>(&event)->GetYCoord();
		break;
	}
	case EventType::DelegateResolutionEvent:
	{
		m_pWindowWidth = static_cast<float>(static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowWidth());
		m_pWindowHeight = static_cast<float>(static_cast<DelegateResolutionEvent*>(&event)->GetClientWindowHeight());
		this->UpdateModules();
		break;
	}
	case EventType::MouseButtonEvent:
	{
		m_pMouseX = static_cast<MouseButtonEvent*>(&event)->GetXCoord();
		m_pMouseY = static_cast<MouseButtonEvent*>(&event)->GetYCoord();
		KeyState state = static_cast<MouseButtonEvent*>(&event)->GetKeyState();
		int virKey = static_cast<MouseButtonEvent*>(&event)->GetVirtualKeyCode();
		if (virKey == VK_LBUTTON && state == KeyState::KeyPress && m_pOnScreen) {
			//Back button
			if (m_pMouseX > m_pBackTextBox.left && m_pMouseX < m_pBackTextBox.right &&
				m_pMouseY > m_pBackTextBox.top && m_pMouseY < m_pBackTextBox.bottom) {
				m_pOnScreen = false;
				//ToggleBackMenu bm;
				//EventBuss::Get().Delegate(bm);
			}
		}
		break;
	}
	case EventType::WindowCloseEvent:
	{
		this->CleanUp();
	}
	default:
		break;
	}
}

void ControlsUI::CleanUp()
{
}
