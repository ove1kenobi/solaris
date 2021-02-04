#include "ImGuiManager.h"
ImGuiManager::ImGuiManager() noexcept
	: m_ShowDemoWindow{ true }
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	EventBuss::Get().AddListener(this, EventType::ToggleImGuiDemoWindowEvent);
}

ImGuiManager::~ImGuiManager() noexcept
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame() noexcept
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::Render() noexcept
{
	if (m_ShowDemoWindow)
		ImGui::ShowDemoWindow(&m_ShowDemoWindow);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::ToggleImGuiDemoWindowEvent :
	{
		m_ShowDemoWindow = !m_ShowDemoWindow;
	}
		break;
	}
}
