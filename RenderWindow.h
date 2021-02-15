#pragma once

#include <sstream>
#include "EventSystem\EventPublisher.h"
#include "EventSystem\WindowEvents.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem\RenderEvents.h"
#include "ImGui\imgui_impl_win32.h"


class RenderWindow : public EventPublisher
{
private:
	HWND m_winHandle;
	static UINT m_clientWinWidth, m_clientWinHeight;
	LPCWSTR windowTitle;
private:
	RenderWindow& operator=(const RenderWindow&) = delete;
	static void CloseWindow(const HWND& hwnd) noexcept;
public:
	static const UINT DEFAULT_WIN_WIDTH = 1920u;
	static const UINT DEFAULT_WIN_HEIGHT = 1080u;
public:
	RenderWindow();
	virtual ~RenderWindow() = default;
	HWND GetHandle();
	UINT GetWidth();
	UINT GetHeight();
	void SetFPS(int fps);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};