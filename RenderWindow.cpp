#include "RenderWindow.h"

RenderWindow::RenderWindow()
{
    LPCWSTR className = L"Window Class";
    LPCWSTR windowTitle = L"Window";
    m_clientWinWidth = 160u;
    m_clientWinHeight = 100u;
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
        CW_USEDEFAULT, CW_USEDEFAULT, winRect.right - winRect.left, winRect.bottom - winRect.top,
        nullptr,                                        // Parent window
        nullptr,                                        // Menu
        (HINSTANCE)GetModuleHandle(nullptr),            // Instance handle
        nullptr                                         // Additional application data
    );
    ShowWindow(m_winHandle, SW_SHOWNORMAL);
}

LRESULT RenderWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
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
        }
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