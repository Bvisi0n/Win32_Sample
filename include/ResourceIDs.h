#ifndef RESOURCEIDS_H
#define RESOURCEIDS_H

// ------ Project wide settings ---------------------
#include "config.h"

namespace ID
{	// Every WORD should be unique across all enumerations
	using UnderlyingType = WORD;

	static_assert(sizeof(UnderlyingType) <= sizeof(uintptr_t),
		"ERROR: ID::UnderlyingType is too large to be packed into an HMENU (Win32 requirement).");

	enum class MenuBar : UnderlyingType
	{	// 0 - 99
		AliceBlue			= 0,
		Lavender			= 1,
		MintCream			= 2,
		PeachPuff			= 3
	};

	enum class CursorModule : UnderlyingType
	{	// 100 - 109
		ArrowRadioButton	= 100,
		HandRadioButton		= 101,
		CrossRadioButton	= 102
	};

	enum class DatePickerModule : UnderlyingType
	{	// 110 - 119
		DatePicker			= 110
	};

	enum class PopUpModule : UnderlyingType
	{	// 120 - 129
		ShowButton			= 120,
		Textbox				= 121
	};

	template <typename T>
	concept ResourceID = std::is_enum_v<T> && sizeof(T) <= sizeof(UnderlyingType);

	template <ResourceID T>
	constexpr HMENU ToHandle(T id)
	{
		return reinterpret_cast<HMENU>(static_cast<uintptr_t>(id));
	}
}

#endif
