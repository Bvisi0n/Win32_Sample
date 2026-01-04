#ifndef RESOURCEIDS_H
#define RESOURCEIDS_H

// ------ Project wide settings ---------------------
#include "config.h"

namespace ID
{	// Every WORD should be unique across all enumerations
	enum class MenuBar : WORD
	{
		AliceBlue		= 1001,
		Lavender		= 1002,
		MintCream		= 1003,
		PeachPuff		= 1004
	};
	enum class PopUpModule : WORD
	{
		ShowButton		= 1101,
		Textbox			= 1102
	};
}

#endif
