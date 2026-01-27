#ifndef SCOPEDCOMCONTEXT_H
#define SCOPEDCOMCONTEXT_H
// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>
#include <combaseapi.h>
#include <commctrl.h>

struct ScopedComContext
{
    ScopedComContext()
    {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES };
        InitCommonControlsEx(&icc);
    }

    ~ScopedComContext() { CoUninitialize(); }

    ScopedComContext(const ScopedComContext&) = delete;
    ScopedComContext(ScopedComContext&&) = delete;
    ScopedComContext& operator=(const ScopedComContext&) = delete;
    ScopedComContext& operator=(ScopedComContext&&) = delete;
};

#endif
