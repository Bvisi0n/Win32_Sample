// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>

// ------ Homebrew ----------------------------------
#include "ScopedComContext.h"
#include "windows/MainWindow.h"

#ifdef _DEBUG
int main()
{
    return wWinMain(GetModuleHandleW(nullptr), nullptr, GetCommandLineW(), SW_SHOWDEFAULT);
}
#endif

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow)
{
    ScopedComContext comGuard{};

    MainWindow mainWindow;
    if (!mainWindow.Create(L"Win32_Sample", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN))
    {
        return 0;
    }

    ShowWindow(mainWindow.Window(), nCmdShow);

    MSG message = { };
    while (GetMessage(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}
