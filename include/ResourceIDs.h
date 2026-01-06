#ifndef RESOURCEIDS_H
#define RESOURCEIDS_H

// ------ Project wide settings ---------------------
#include "config.h"

namespace ID
{	// Every WORD should be unique across all enumerations
	using UnderlyingType = WORD;

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

	enum class PopUpModule : UnderlyingType
	{	// 110 - 119
		ShowButton			= 110,
		Textbox				= 111
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
