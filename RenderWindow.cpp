#include "RenderWindow.h"

RenderWindow::RenderWindow()
{
    LPCWSTR className = L"Window Class";
    LPCWSTR windowTitle = L"Window";
    m_clientWinWidth = DEFAULT_WIN_WIDTH;
    m_clientWinHeight = DEFAULT_WIN_HEIGHT;
    WNDCLASSEX wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;                  //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
    wc.lpfnWndProc = WindowProc;                         //Pointer to Window Proc function for handling messages from this window
    wc.cbClsExtra = 0;                                   //# of extra bytes to allocate following the window-class structure. We are not currently using this.
    wc.cbWndExtra = 0;                                   //# of extra bytes to allocate following the window instance. We are not currently using this.
    wc.hInstance = (HINSTANCE)GetModuleHandle(nullptr);  //Handle to the instance that contains the Window Procedure
    wc.hIcon = nullptr;                                  //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
    wc.hIconSm = nullptr;                                //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);         //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;             //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
    wc.lpszMenuName = nullptr;                           //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
    wc.lpszClassName = className;                        //Pointer to null terminated string of our class name for this window.
    wc.cbSize = sizeof(WNDCLASSEX);                      //Need to fill in the size of our struct for cbSize

    RegisterClassEx(&wc);

    RECT winRect = {0, 0, static_cast<LONG>(m_clientWinWidth), static_cast<LONG>(m_clientWinHeight)};

    m_winHandle = CreateWindowEx(
        0,                                              // Optional window style
        className,
        windowTitle,
        WS_OVERLAPPEDWINDOW,                            // Window style
        // x positoin, y positoin, width, height
        0u, 0u, winRect.right - winRect.left, winRect.bottom - winRect.top,
        nullptr,                                        // Parent window
        nullptr,                                        // Menu
        (HINSTANCE)GetModuleHandle(nullptr),            // Instance handle
        nullptr                                         // Additional application data
    );
    ShowWindow(m_winHandle, SW_SHOWNORMAL);

    AllocConsole();                                        // Remove
    freopen("CONOUT$", "w", stdout);                       // Remove
    std::cout << "Debug console is open" << std::endl;     // Remove
}

LRESULT RenderWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            WindowCloseEvent ce;
            EventBuss::Get().Delegate(ce);
            return 0;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CLOSE:
        {
            if (MessageBox(hwnd, L"Quit?", L"Exit", MB_YESNO) == IDYES)
                DestroyWindow(hwnd);
            else
                return 0;
            break;
        }
        case WM_MOUSEMOVE:
        {
            // mouse moved within window
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            MouseMoveEvent me(xPos, yPos);
            EventBuss::Get().Delegate(me);
            return 0;
        }
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
        case WM_KEYDOWN:
        {
            // key press
#if defined(DEBUG) | defined(_DEBUG)
            if (wParam == 'P') {
                ToggleWireFrameEvent event;
                EventBuss::Get().Delegate(event);
            }
#endif

            KeyState keyState;
            if ((lParam & 0x40000000) == 0) keyState = KeyState::KeyPress;
            else keyState = KeyState::KeyRepeat;
            KeyboardEvent ke(keyState, (int)wParam);
            EventBuss::Get().Delegate(ke);
            return 0;
        }
        case WM_KEYUP:
        {
            // key release
            KeyboardEvent ke(KeyState::KeyRelease, (int)wParam);
            EventBuss::Get().Delegate(ke);
            return 0;
        }
        /*
        case WM_KEYDOWN :
            switch (wParam)
            {
                //P Key
            case 0x50:
            {
                #if defined(DEBUG) | defined(_DEBUG)
                ToggleWireFrameEvent event;
                EventBuss::Get().Delegate(event);
                #endif
            }
                break;
            }*/
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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