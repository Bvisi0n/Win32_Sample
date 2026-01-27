#ifndef IWINDOW_H
#define IWINDOW_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Forward Declarations ----------------------
struct HWND__;
typedef struct HWND__* HWND;

class IWindow
{
public:
    virtual ~IWindow() = default;
    virtual HWND Window() const = 0; // Pure virtual
};

#endif
