#ifndef RESOURCEIDS_H
#define RESOURCEIDS_H

// ------ Project wide settings ---------------------
#include "config.h"

namespace ID
{	// Every WORD should be unique across all enumerations
	using UnderlyingType = WORD;

	enum class MenuBar : UnderlyingType
	{	// 0000 - 0099
		AliceBlue			= 0000,
		Lavender			= 0001,
		MintCream			= 0002,
		PeachPuff			= 0003
	};

	enum class CursorModule : UnderlyingType
	{	// 0100 - 0109
		ArrowRadioButton	= 0100,
		HandRadioButton		= 0101,
		CrossRadioButton	= 0102
	};

	enum class PopUpModule : UnderlyingType
	{	// 0110 - 0119
		ShowButton			= 0110,
		Textbox				= 0111
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
