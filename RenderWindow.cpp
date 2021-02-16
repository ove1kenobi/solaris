#include "pch.h"
#include "RenderWindow.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

UINT RenderWindow::m_clientWinWidth = DEFAULT_WIN_WIDTH;
UINT RenderWindow::m_clientWinHeight = DEFAULT_WIN_HEIGHT;

RenderWindow::RenderWindow()
{
    LPCWSTR className = L"Window Class";
    windowTitle = L"Window, FPS: ";
    WNDCLASSEX wc = { 0 };
    wc.style = CS_OWNDC;                                 //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
    wc.lpfnWndProc = WindowProc;                         //Pointer to Window Proc function for handling messages from this window
    wc.cbClsExtra = 0;                                   //# of extra bytes to allocate following the window-class structure. We are not currently using this.
    wc.cbWndExtra = 0;                                   //# of extra bytes to allocate following the window instance. We are not currently using this.
    wc.hInstance = (HINSTANCE)GetModuleHandle(nullptr);  //Handle to the instance that contains the Window Procedure
    wc.hIcon = nullptr;                                  //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
    wc.hIconSm = nullptr;                                //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
    wc.hCursor =  nullptr;                                //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
    wc.hbrBackground = nullptr;                          //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
    wc.lpszMenuName = nullptr;                           //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
    wc.lpszClassName = className;                        //Pointer to null terminated string of our class name for this window.
    wc.cbSize = sizeof(WNDCLASSEX);                      //Need to fill in the size of our struct for cbSize

    RegisterClassEx(&wc);

    RECT winRect = {0, 0, static_cast<LONG>(m_clientWinWidth), static_cast<LONG>(m_clientWinHeight)};
    winRect.left = 100;
    winRect.right = m_clientWinWidth + winRect.left;
    winRect.top = 100;
    winRect.bottom = m_clientWinHeight + winRect.top;
    AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    m_winHandle = CreateWindowEx(0,                                              // Optional window style
                                 className,
                                 windowTitle,
                                 WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,       // Window style
                                 0, 0, winRect.right - winRect.left, winRect.bottom - winRect.top,
                                 nullptr,                                        // Parent window
                                 nullptr,                                        // Menu
                                 (HINSTANCE)GetModuleHandle(nullptr),            // Instance handle
                                 nullptr);                                       // Additional application data
    ShowWindow(m_winHandle, SW_SHOWNORMAL);
    //winRect.top += 62;
    //winRect.left += 15;
    //winRect.bottom += 23;
    //ClipCursor(&winRect);
    //ShowCursor(showCursor);

    // Register mouse as raw input device
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = NULL;

    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) {
        // handle error
    }

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "Debug console is open" << std::endl;
}

LRESULT RenderWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            WindowCloseEvent ce;
            EventBuss::Get().Delegate(ce);
            break;
        }
        case WM_CLOSE:
        {
            CloseWindow(hwnd);
            break;
        }
        /*
       
        case WM_LBUTTONDOWN:
        {
            // left mouse butten down
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseButtenEvent be(KeyState::KeyPress, VK_LBUTTON, xPos, yPos);
            EventBuss::Get().Delegate(be);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            // left mouse butten up
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseButtenEvent be(KeyState::KeyRelease, VK_LBUTTON, xPos, yPos);
            EventBuss::Get().Delegate(be);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            // right mouse butten down
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseButtenEvent be(KeyState::KeyPress, VK_RBUTTON, xPos, yPos);
            EventBuss::Get().Delegate(be);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            // right mouse butten up
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseButtenEvent be(KeyState::KeyRelease, VK_RBUTTON, xPos, yPos);
            EventBuss::Get().Delegate(be);
            return 0;
        }
        *//*
        case WM_INPUT:
        {
            UINT dataSize = 0;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

            if (dataSize > 0)
            {
                std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
                if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
                {
                    RAWINPUT* raw = (RAWINPUT*)rawdata.get();
                    if (raw->header.dwType == RIM_TYPEMOUSE)
                    {
                        int xDiff = raw->data.mouse.lLastX;
                        int yDiff = raw->data.mouse.lLastY;
                        MouseMoveRelativeEvent mre(xDiff, yDiff);
                        EventBuss::Get().Delegate(mre);
                    }
                }
            }

            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }*/
        case WM_MOUSEMOVE:
        {
            float xPos = GET_X_LPARAM(lParam);
            float yPos = GET_Y_LPARAM(lParam);
            xPos = 2.0f*xPos / (float)m_clientWinWidth - 1.0f;
            yPos = 2.0f*yPos / (float)m_clientWinHeight - 1.0f;
            MouseMoveAbsoluteEvent mae(xPos, yPos);
            EventBuss::Get().Delegate(mae);
        }
        case WM_MOUSEWHEEL:
        {
            int wheelScroll = GET_WHEEL_DELTA_WPARAM(wParam);
            MouseScrollEvent se(wheelScroll);
            EventBuss::Get().Delegate(se);
            break;
        }

        case WM_MOUSEHOVER:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseMoveAbsoluteEvent mae(xPos, yPos);
            EventBuss::Get().Delegate(mae);
            break;
        }
        case WM_KEYDOWN:
        {
            // key press
            #if defined(DEBUG) | defined(_DEBUG)
            if (wParam == 'P') {
                ToggleWireFrameEvent event;
                EventBuss::Get().Delegate(event);
            }
            if (wParam == 'I')
            {

                ToggleImGuiEvent imEvent;
                EventBuss::Get().Delegate(imEvent);
            }
            #endif
            if (wParam == VK_ESCAPE)
            {
                CloseWindow(hwnd);
                break;
            }
            KeyState keyState;
            if ((lParam & 0x40000000) == 0) keyState = KeyState::KeyPress;
            else keyState = KeyState::KeyRepeat;
            KeyboardEvent ke(keyState, (int)wParam);
            EventBuss::Get().Delegate(ke);
            break;
        }
        case WM_KEYUP:
        {
            // key release
            KeyboardEvent ke(KeyState::KeyRelease, (int)wParam);
            EventBuss::Get().Delegate(ke);
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void RenderWindow::CloseWindow(const HWND& hwnd) noexcept
{
    if (MessageBox(hwnd, L"Quit?", L"Exit", MB_YESNO) == IDYES)
        DestroyWindow(hwnd);
}

HWND RenderWindow::GetHandle()
{
    return m_winHandle;
}

UINT RenderWindow::GetWidth()
{
    return m_clientWinWidth;
}

UINT RenderWindow::GetHeight()
{
    return m_clientWinHeight;
}

void RenderWindow::SetFPS(int fps)
{
    std::wstringstream wss;
    wss << fps;
    std::wstring title = std::wstring(windowTitle) + wss.str();
    SetWindowText(m_winHandle, title.c_str());
}
