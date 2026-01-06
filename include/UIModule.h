#ifndef UIMODULE_H
#define UIMODULE_H

// ------ Project wide settings ---------------------
#include "config.h"

class UIModule {
public:
			UIModule()							 = default;
	virtual ~UIModule()							 = default;
			UIModule(const UIModule&)			 = delete;
			UIModule(UIModule&&)				 = delete;
			UIModule& operator=(const UIModule&) = delete;
			UIModule& operator=(UIModule&&)		 = delete;

	// TODO: Return std::expected<int> to return std::unexpected(GetLastError())
	[[nodiscard]] virtual int Initialize(HWND ownerHandle, float dpiScale, int yPosition) = 0;
    [[nodiscard]] virtual int UpdateLayout(float dpiScale, int yPosition) const = 0;
	// TODO: Add GetCursor() OnCommand(), OnNotify()
	// This would allow wrapping all UIModules in a std::array, cleanly abstract logic away from MainWindow, make it even cleaner

protected:
	HWND m_ParentHandle = nullptr; // Keeps UpdateLayout logic abstracted away from the parent
};

#endif
