#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

class RenderWindow
{
private:
	HWND m_winHandle;
private:
	RenderWindow& operator=(const RenderWindow&) = delete;

public:
	RenderWindow();
	HWND GetHandle();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
