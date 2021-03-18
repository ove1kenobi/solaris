#include "..\pch.h"
#include "ControlsUI.h"

bool ControlsUI::CreateScreen()
{
    return false;
}

bool ControlsUI::UpdateScreen()
{
    return false;
}

bool ControlsUI::UpdateModules()
{
    return false;
}

void ControlsUI::RenderScreen()
{
}

ControlsUI::ControlsUI() noexcept
{
}

ControlsUI::~ControlsUI()
{
}

bool ControlsUI::Initialize()
{
    return false;
}

void ControlsUI::Render()
{
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
			/*
			//Start game
			if (m_pMouseX > m_pStartTextBox.left && m_pMouseX < m_pStartTextBox.right &&
				m_pMouseY > m_pStartTextBox.top && m_pMouseY < m_pStartTextBox.bottom) {
				m_pOnScreen = false;
				ToggleStartGame sg;
				EventBuss::Get().Delegate(sg);
			}
			//Exit game
			if (m_pMouseX > m_pExitTextBox.left && m_pMouseX < m_pExitTextBox.right &&
				m_pMouseY > m_pExitTextBox.top && m_pMouseY < m_pExitTextBox.bottom) {
				m_pOnScreen = false;
				WindowCloseEvent ce;
				EventBuss::Get().Delegate(ce);
			}*/
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
