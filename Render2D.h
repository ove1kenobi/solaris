#pragma once
#include "pch.h"
#include "UI/TypesUI.h"
#include "UI/PlanetInteractionUI.h"
#include "UI/HeadsUpDisplayUI.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem\UtilityEvents.h"
#include "EventSystem\PlayerEvents.h"
#include "Player.h"
//Direct2D will be used to render all of the graphics, and use XAML for everything else UI related (Layout, interactions).
//XAML: https://docs.microsoft.com/en-us/windows/uwp/gaming/directx-and-xaml-interop
//Direct2D: https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview
#define DISTANCE_THRESHOLD 800.0f
class Render2D : public EventPublisher, IEventListener {
private:
	TypesUI m_CurrentUI;
	std::vector<ModuleUI*> m_Modules;
	PlayerInfo* m_pPlayerInfo;
	bool m_PlanetInteractionUIOpen;
	bool m_Render;
	std::wstring GetFontFilePath(std::wstring fontFile);
	bool AddFonts();
public:
	Render2D() noexcept;
	virtual ~Render2D();

	[[nodiscard]] const bool Initialize() noexcept;
	void RenderUI();
	void OnEvent(IEvent& event) noexcept;
};
