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
		// Create the Background Color menu
		HMENU bgColorSubMenu = CreatePopupMenu();
		AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::AliceBlue), L"Alice Blue");
		AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::Lavender), L"Lavender");
		AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::MintCream), L"Mint Cream");
		AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::PeachPuff), L"Peach Puff");

		// Create the View menu
		HMENU viewMenu = CreatePopupMenu();
		AppendMenuW(viewMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(bgColorSubMenu), L"Background Color");

		// Create the File menu
		HMENU fileMenu = CreatePopupMenu();
		AppendMenuW(fileMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::Load), L"Load");
		AppendMenuW(fileMenu, MF_STRING, static_cast<UINT_PTR>(ID::MenuBar::Save), L"Save");

		// Create the actual Menu Bar
		m_MenuHandle = CreateMenu();
		AppendMenuW(m_MenuHandle, MF_POPUP, reinterpret_cast<UINT_PTR>(fileMenu), L"File");
		AppendMenuW(m_MenuHandle, MF_POPUP, reinterpret_cast<UINT_PTR>(viewMenu), L"View");

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
