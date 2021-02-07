#pragma once
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "..\EventSystem\IEventListener.h"
#include "..\EventSystem\EventBuss.h"
class ImGuiManager : public IEventListener
{
private:
	bool m_ShowDemoWindow;
public:
	ImGuiManager() noexcept;
	virtual ~ImGuiManager() noexcept;
	void BeginFrame() noexcept;
	void Render() noexcept;
	void OnEvent(IEvent& event) noexcept;
};