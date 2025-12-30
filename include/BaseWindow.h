#ifndef BASEWINDOW_H
#define BASEWINDOW_H

// Project wide settings
#include "Config.h"

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL; // Holds the instance pointer for this window

        if (uMsg == WM_NCCREATE)    // Handle creation message so we can associate the OS window with the object
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;          // Unpack the parameter with the correct struct
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;         // Retrieve the this pointer for the instance being created
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis); // Store the instance pointer in the window’s user data so later messages can find it

            pThis->m_hwnd = hwnd; // Save the window handle locally
        }
        else        // Handle all other messages
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA); // Retrieve the instance pointer
        }
        if (pThis)  // If Window exists
        {           // -> Handle message
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else        // If window doesn't exist
        {           // -> Guards against dereferencing a null pointer if a message arrives before WM_NCCREATE
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) {}

    BOOL Create(
        PCWSTR lpWindowName,            // Window text
        DWORD dwStyle,                  // Window style
        DWORD dwExStyle = 0,            // Optional window styles
        int x = CW_USEDEFAULT,          // Window x position
        int y = CW_USEDEFAULT,          // Window y position
        int nWidth = CW_USEDEFAULT,     // Window width
        int nHeight = CW_USEDEFAULT,    // Window height
        HWND hWndParent = 0,            // Parent window
        HMENU hMenu = 0                 // Menu
    )
    {
        WNDCLASS wc = {};

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;  // Pointer to function
        wc.hInstance = GetModuleHandle(NULL);       // Handle to application instance
        wc.lpszClassName = ClassName();             // String that identifies window class

        RegisterClass(&wc);                         // Registers the window class with the OS

        m_hwnd = CreateWindowEx(
            dwExStyle,              // Optional window styles
            ClassName(),            // Window class
            lpWindowName,           // Window text
            dwStyle,                // Window style
            
            // Size and position
            x, y, nWidth, nHeight,
            
            hWndParent,             // Parent window
            hMenu,                  // Menu
            GetModuleHandle(NULL),  // Instance handle
            this                    // Additional application data
        );

        return (m_hwnd ? TRUE : FALSE); // Check if Window was successfully created
    }

    // Return the window handle
    HWND Window() const { return m_hwnd; }

protected:

    // Returns a string that identifies the window class
    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};

#endif
