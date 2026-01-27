// ------ Win32 and more ----------------------------
#include <windows.h>

// ------ Homebrew ----------------------------------
#include "controls/MenuBar.h"

MenuBar::MenuBar()
	: m_MenuHandle(nullptr) {}

MenuBar::~MenuBar()
{
	if (m_MenuHandle && !GetMenuItemCount(m_MenuHandle))
	{
		DestroyMenu(m_MenuHandle);
	}
}

void MenuBar::Initialize(HWND owner)
{
	// Create the Background Color menu
	HMENU bgColorSubMenu = CreatePopupMenu();
	AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(UI::ControlID::AliceBlue), L"Alice Blue");
	AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(UI::ControlID::Lavender), L"Lavender");
	AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(UI::ControlID::MintCream), L"Mint Cream");
	AppendMenuW(bgColorSubMenu, MF_STRING, static_cast<UINT_PTR>(UI::ControlID::PeachPuff), L"Peach Puff");

	// Create the View menu
	HMENU viewMenu = CreatePopupMenu();
	AppendMenuW(viewMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(bgColorSubMenu), L"Background Color");

	// Create the File menu
	HMENU fileMenu = CreatePopupMenu();
	AppendMenuW(fileMenu, MF_STRING, static_cast<UINT_PTR>(UI::ControlID::Load), L"Load");
	AppendMenuW(fileMenu, MF_STRING, static_cast<UINT_PTR>(UI::ControlID::Save), L"Save");

	// Create the actual Menu Bar
	m_MenuHandle = CreateMenu();
	AppendMenuW(m_MenuHandle, MF_POPUP, reinterpret_cast<UINT_PTR>(fileMenu), L"File");
	AppendMenuW(m_MenuHandle, MF_POPUP, reinterpret_cast<UINT_PTR>(viewMenu), L"View");

	SetMenu(owner, m_MenuHandle);
}

std::optional<D2D1_COLOR_F> MenuBar::GetColorFromID(UI::ControlID id) const
{
	switch (id)
	{
		case UI::ControlID::AliceBlue:
			return D2D1::ColorF(D2D1::ColorF::AliceBlue);
		case UI::ControlID::Lavender:
			return D2D1::ColorF(D2D1::ColorF::Lavender);
		case UI::ControlID::MintCream:
			return D2D1::ColorF(D2D1::ColorF::MintCream);
		case UI::ControlID::PeachPuff:
			return D2D1::ColorF(D2D1::ColorF::PeachPuff);
	}
	return std::nullopt;
}
