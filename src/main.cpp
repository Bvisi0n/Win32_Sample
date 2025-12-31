// Project wide settings
#include "Config.h"

// Homebrew
#include "MainWindow.h"

#ifdef _DEBUG
int main()
{
    return wWinMain(GetModuleHandleW(nullptr), nullptr, GetCommandLineW(), SW_SHOWDEFAULT);
}
#endif

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow)
{
    MainWindow mainWindow;

    if (!mainWindow.Create(L"Win32_Sample", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(mainWindow.Window(), nCmdShow);

    // Run the message loop

    MSG message = { };
    while (GetMessage(&message, nullptr, 0, 0) > 0) // The other 3 parameters filter which messages you get from the queue
    {   // GetMessage must be inside the condition, i.e; when WM_QUIT is received the loop terminates
        TranslateMessage(&message); // Translates keystrokes (key down, key up) into characters
        DispatchMessage(&message);  // Tells OS to call the WindowProc of the window that's the target of the message
    }

    return 0;  // End of program
}
