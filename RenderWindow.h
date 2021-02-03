#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include "EventSystem\EventPublisher.h"
#include "EventSystem\WindowEvents.h"
#include "EventSystem/MouseMoveEvent.h"
#include "EventSystem/MouseButtenEvent.h"
#include "EventSystem/KeyboardEvent.h"

// Debug console
//#include <iostream>			
//#pragma warning(disable : 4996)

#include "EventSystem\RenderEvents.h"
#include "ImGui\imgui_impl_win32.h"
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
	virtual ~RenderWindow();
	HWND GetHandle();
	UINT GetWidth();
	UINT GetHeight();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
