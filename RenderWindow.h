#pragma once
#include "EventSystem\EventPublisher.h"
#include "EventSystem\WindowEvents.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem\RenderEvents.h"
#include "DirectXTK/Mouse.h"
#include "ImGui\imgui_impl_win32.h"

class RenderWindow : public EventPublisher
{
private:
	HWND m_winHandle;
	static UINT m_clientWinWidth, m_clientWinHeight;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	static bool m_DisableXTKMouse;
private:
	RenderWindow& operator=(const RenderWindow&) = delete;
	static void ToggleXTKMouse() noexcept;
	static void CloseWindow(const HWND& hwnd) noexcept;
public:
	static const UINT DEFAULT_WIN_WIDTH = 1200u;
	static const UINT DEFAULT_WIN_HEIGHT = 800u;
public:
	RenderWindow();
	virtual ~RenderWindow() = default;
	HWND GetHandle();
	UINT GetWidth();
	UINT GetHeight();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};