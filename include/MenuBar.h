#ifndef MENUBAR_H
#define MENUBAR_H
// ------ Project wide settings ---------------------
#include "config.h"

// ------ STL ---------------------------------------
#include <optional>

class MenuBar {
public:
	enum class ID : WORD // May have to extract this out in a more global scope later
	{
		AliceBlue	= 1001,
		Lavender	= 1002,
		MintCream	= 1003,
		PeachPuff	= 1004
	};

	MenuBar() = default;

	void Initialize(HWND owner)
	{
		m_MenuHandle = CreateMenu();
		HMENU hBackgroundMenu = CreateMenu();

		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::AliceBlue),	L"Alice Blue");
		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::Lavender),	L"Lavender");
		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::MintCream),	L"Mint Cream");
		AppendMenuW(hBackgroundMenu, MF_STRING, static_cast<UINT_PTR>(ID::PeachPuff),	L"Peach Puff");

		AppendMenuW(m_MenuHandle, MF_POPUP, reinterpret_cast<UINT_PTR>(hBackgroundMenu), L"Background Color");

		SetMenu(owner, m_MenuHandle);
	}

	std::optional<D2D1_COLOR_F> GetColorFromID(ID id)
	{
		switch (id)
		{
			case MenuBar::ID::AliceBlue:
				return D2D1::ColorF(D2D1::ColorF::AliceBlue);
			case MenuBar::ID::Lavender:
				return D2D1::ColorF(D2D1::ColorF::Lavender);
			case MenuBar::ID::MintCream:
				return D2D1::ColorF(D2D1::ColorF::MintCream);
			case MenuBar::ID::PeachPuff:
				return D2D1::ColorF(D2D1::ColorF::PeachPuff);
		}
		return std::nullopt;
	}

private:
	HMENU m_MenuHandle = nullptr;
};

#endif
