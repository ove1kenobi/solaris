#pragma once
#include "pch.h"
#include "UI/TypesUI.h"
#include "UI/PlanetInteractionUI.h"
#include "UI/HeadsUpDisplayUI.h"
#include "UI/MenuUI.h"
#include "UI/PressInteractUI.h"
#include "UI/CrosshairUI.h"
#include "UI/UpgradeScreenUI.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem\UtilityEvents.h"
#include "EventSystem\PlayerEvents.h"
#include "Player.h"

#define DISTANCE_THRESHOLD 800.0f
class Render2D : public EventPublisher, IEventListener {
private:
	TypesUI m_CurrentUI;
	std::vector<ModuleUI*> m_Modules;
	PlayerInfo* m_pPlayerInfo;

	bool m_PlanetInteraction;
	bool m_UpgradeScreen;
	bool m_InGame;
	std::wstring GetFontFilePath(std::wstring fontFile);
	bool AddFonts();
public:
	Render2D() noexcept;
	virtual ~Render2D();

	[[nodiscard]] const bool Initialize() noexcept;
	void RenderUI();
	void OnEvent(IEvent& event) noexcept;
	void CleanUp();
};
