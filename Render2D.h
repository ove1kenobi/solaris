#pragma once
#include "pch.h"
#include "UI/TypesUI.h"
#include "UI/PlanetInteractionUI.h"
#include "UI/HeadsUpDisplayUI.h"
#include "UI/MenuUI.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/InputEvents.h"

class Render2D : public EventPublisher, IEventListener {
private:
	TypesUI m_CurrentUI;
	std::vector<ModuleUI*> m_Modules;

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
