#include "config.h" // Handles #include<windows.h> and it's "settings"

void OnSize(HWND hwnd, UINT flag, int width, int height);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main()
{
    return wWinMain(GetModuleHandleW(nullptr), nullptr, GetCommandLineW(), SW_SHOWDEFAULT);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow)
{
    // Register the window class
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;    // Pointer to function
    wc.hInstance = hInstance;       // Handle to application instance
    wc.lpszClassName = CLASS_NAME;  // String that identifies window class

    RegisterClass(&wc);             // Registers the window class with the operating system

    // Create the window

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0; // Failed to make a window -> terminate
    }

    ShowWindow(hwnd, nCmdShow); // nCmdShow can be used to minimize or maximize a window

    // Run the message loop

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) // The other 3 parameters filter which messages you get from the queue
    {   // GetMessage must be inside the condition, i.e; when WM_QUIT is received the loop terminates
        TranslateMessage(&msg); // Translates keystrokes (key down, key up) into characters
        DispatchMessage(&msg);  // Tells OS to call the WindowProc of the window that's the target of the message
    }

    return 0;  // End of program
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        {
            if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd); // Deactivates window if user confirms, sends WM_DESTROY
            }
            return 0; // Message handled -> exit function
        }
        case WM_DESTROY:    // Sent after the window is deactivated, but before the destruction occurs
        {                   // In particular, before any child windows are destroyed
            if (GetWindow(hwnd, GW_OWNER) == NULL)
            {   // If hwnd doesn't have a parent, then it must be the main window
                PostQuitMessage(0); //Adds WM_QUIT to queue which will terminate message loop/program
            }
            return 0; // Message handled -> exit function
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps); // Fills the PAINTSTRUCT with repaint request info
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1)); // FillRect fills a rectangle by using the specified brush
            EndPaint(hwnd, &ps); //  Clears update region, signals to OS that window has completed painting itself
            return 0; // Message handled -> exit function
        }
        case WM_SIZE:
        {
            int width = LOWORD(lParam);     // Macro to get the low-order word
            int height = HIWORD(lParam);    // Macro to get the high-order word
            OnSize(hwnd, (UINT)wParam, width, height); // Respond to the message
            return 0; // Message handled -> exit function
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam); // Performs the default action for the message
}

void OnSize(HWND hwnd, UINT flag, int width, int height)
{
    // Handle resizing
}
