#ifndef RESOURCEIDS_H
#define RESOURCEIDS_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>	// The core

namespace ID
{	// Every WORD should be unique across all enumerations
	using UnderlyingType = WORD;

	static_assert(sizeof(UnderlyingType) <= sizeof(uintptr_t),
		"ERROR: ID::UnderlyingType is too large to be packed into an HMENU (Win32 requirement).");

	enum class MenuBar : UnderlyingType
	{	// 0 - 99
		Load				= 0,
		Save				= 1,
		AliceBlue			= 2,
		Lavender			= 3,
		MintCream			= 4,
		PeachPuff			= 5
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

	enum class FileSelectModule : UnderlyingType
	{	// 120 - 129
		InfoLabel			= 120,
		SelectButton		= 121
	};

	enum class PopUpModule : UnderlyingType
	{	// 120 - 129
		ShowButton			= 130,
		Textbox				= 131
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
