#pragma once
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "..\Layer.h"
#include "..\EventSystem\EventBuss.h"
class ImGuiManager : public Layer
{
private:
	bool m_ShowDemoWindow;
private:
	void OnEvent(IEvent& event) noexcept override;
public:
	ImGuiManager() noexcept;
	virtual ~ImGuiManager() noexcept;
	void BeginFrame() noexcept;
	void Render() noexcept;
	[[nodiscard]] const std::string GetDebugName() const noexcept override;
};