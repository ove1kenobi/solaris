#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "EventSystem\EventPublisher.h"
#include "EventSystem\WindowEvents.h"
class RenderWindow : public EventPublisher
{
private:
	HWND m_winHandle;
	UINT m_clientWinWidth, m_clientWinHeight;
private:
	RenderWindow& operator=(const RenderWindow&) = delete;
public:
	static const UINT DEFAULT_WIN_WIDTH = 1200u;
	static const UINT DEFAULT_WIN_HEIGHT = 800u;
public:
	RenderWindow();
	~RenderWindow() = default;
	HWND GetHandle();
	UINT GetWidth();
	UINT GetHeight();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
