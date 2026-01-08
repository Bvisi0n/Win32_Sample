#ifndef MENUBAR_H
#define MENUBAR_H
// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>	// The core
#include <d2d1.h>		// Direct2D

// ------ STL ---------------------------------------
#include <optional>

// ------ Homebrew ----------------------------------
#include "ResourceIDs.h"

class MenuBar {
public:
	MenuBar()							= default;
	~MenuBar()							= default;
	MenuBar(const MenuBar&)				= delete;
	MenuBar(MenuBar&&)					= delete;
	MenuBar& operator=(const MenuBar&)	= delete;
	MenuBar& operator=(MenuBar&&)		= delete;

	void Initialize(HWND owner)
	{
		m_MenuHandle			= CreateMenu();
		HMENU hBackgroundMenu	= CreateMenu();
		
		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::AliceBlue),	L"Alice Blue");
		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::Lavender),	L"Lavender");
		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::MintCream),	L"Mint Cream");
		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::PeachPuff),	L"Peach Puff");

		AppendMenuW(m_MenuHandle,	 MF_POPUP,  reinterpret_cast<UINT_PTR>(hBackgroundMenu),	L"Background Color");

		SetMenu(owner, m_MenuHandle);
	}

	std::optional<D2D1_COLOR_F> GetColorFromID(ID::MenuBar id) const
	{
		switch (id)
		{
			case ID::MenuBar::AliceBlue:
				return D2D1::ColorF(D2D1::ColorF::AliceBlue);
			case ID::MenuBar::Lavender:
				return D2D1::ColorF(D2D1::ColorF::Lavender);
			case ID::MenuBar::MintCream:
				return D2D1::ColorF(D2D1::ColorF::MintCream);
			case ID::MenuBar::PeachPuff:
				return D2D1::ColorF(D2D1::ColorF::PeachPuff);
		}
		return std::nullopt;
	}

private:
	HMENU m_MenuHandle = nullptr;
};

#endif
