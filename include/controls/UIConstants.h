#ifndef UICONSTANTS_H
#define UICONSTANTS_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ STL ---------------------------------------
#include <cstdint>
#include <functional>

class SampleWindow;

namespace UI
{
	// Tab order of UI elements is determined by order of their ID's.
	// The ID is used as key in the std::map that holds all the UI elements.
	// The sorted nature of std::map is used to determine the tab order.
	// This means you can change the tab order by changing the ID values.
	enum class ControlID : uint16_t
	{
		// MenuBar -----------------
		Load				= 0,
		Save				= 1,
		AliceBlue			= 2,
		Lavender			= 3,
		MintCream			= 4,
		PeachPuff			= 5,

		// Popup -------------------
		PopUp_Textbox		= 100,
		PopUp_Button		= 101,

		// Cursor ------------------
		Cursor_ArrowButton	= 200,
		Cursor_HandButton	= 201,
		Cursor_CrossButton	= 202,

		// DatePicker --------------
		DatePicker			= 300,

		// FileSelect --------------
		FileSelect_Button	= 400,
		FileSelect_Label	= 401
	};

	// A standard signature for UI actions.
	using Action = std::function<void(SampleWindow*)>;
}

#endif
