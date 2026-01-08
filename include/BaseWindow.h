#ifndef BASEWINDOW_H
#define BASEWINDOW_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>	// The core

template <typename DerivedType>
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        DerivedType* pThis = nullptr; // Holds the instance pointer for this window

        if (message == WM_NCCREATE)    // Handle creation message so we can associate the OS window with the object
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;                  // Unpack the parameter with the correct struct
            pThis = (DerivedType*)pCreate->lpCreateParams;                 // Retrieve the this pointer for the instance being created
            SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)pThis); // Store the instance pointer in the window’s user data so later messages can find it

            pThis->m_WindowHandle = windowHandle; // Save the window handle locally
        }
        else        // Handle all other messages
        {
            pThis = (DerivedType*)GetWindowLongPtr(windowHandle, GWLP_USERDATA); // Retrieve the instance pointer
        }
        if (pThis)  // If Window exists
        {           // -> Handle message
            return pThis->HandleMessage(message, wParam, lParam);
        }
        else        // If window doesn't exist
        {           // -> Guards against dereferencing a null pointer if a message arrives before WM_NCCREATE
            return DefWindowProc(windowHandle, message, wParam, lParam);
        }
    }

            BaseWindow()                                = default;
    virtual ~BaseWindow()                               = default;
            BaseWindow(const BaseWindow&)               = delete;
            BaseWindow(BaseWindow&&)                    = delete;
            BaseWindow& operator=(const BaseWindow&)    = delete;
            BaseWindow& operator=(BaseWindow&&)         = delete;

    BOOL Create(
        PCWSTR windowName,          // Window text
        DWORD style,                // Window style
        DWORD extendedStyle = 0,    // Optional window styles
        int x = CW_USEDEFAULT,      // Window x position
        int y = CW_USEDEFAULT,      // Window y position
        int width = CW_USEDEFAULT,  // Window width
        int height = CW_USEDEFAULT, // Window height
        HWND parent = 0,            // Parent window
        HMENU menu = 0              // Menu
    )
    {
        WNDCLASS windowClass = {};

        windowClass.lpfnWndProc     = BaseWindow::WindowProc;   // Pointer to function
        windowClass.hInstance       = GetModuleHandle(nullptr); // Handle to application instance
        windowClass.lpszClassName   = ClassName();              // String that identifies window class

        RegisterClass(&windowClass);                      // Registers the window class with the OS

        m_WindowHandle = CreateWindowEx(
            extendedStyle,  // Optional window styles
            ClassName(),    // Window class
            windowName,     // Window text
            style,          // Window style
            
            // Size and position
            x, y, width, height,
            
            parent,                     // Parent window handle
            menu,                       // Menu
            GetModuleHandle(nullptr),   // Instance handle
            this                        // Additional application data
        );

        return (m_WindowHandle ? TRUE : FALSE); // Check if Window was successfully created
    }

    // Return the window handle
    HWND Window() const { return m_WindowHandle; }

protected:
    // Returns a string that identifies the window class
    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_WindowHandle = nullptr;
};

#endif
