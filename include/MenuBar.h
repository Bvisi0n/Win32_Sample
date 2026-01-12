#ifndef MENUBAR_H
#define MENUBAR_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <d2d1.h> // I haven't found an alternative for D2D1_COLOR_F

// ------ STL ---------------------------------------
#include <optional>

// ------ Homebrew ----------------------------------
#include "UIConstants.h"

struct HWND__;
typedef struct HWND__* HWND;
struct HMENU__;
typedef struct HMENU__* HMENU;

class MenuBar {
public:
	MenuBar();
	~MenuBar();
	MenuBar(const MenuBar&)				= delete;
	MenuBar(MenuBar&&)					= delete;
	MenuBar& operator=(const MenuBar&)	= delete;
	MenuBar& operator=(MenuBar&&)		= delete;

	void Initialize(HWND owner);
	std::optional<D2D1_COLOR_F> GetColorFromID(UI::ControlID id) const;
	
private:
	HMENU m_MenuHandle;
};

#endif
