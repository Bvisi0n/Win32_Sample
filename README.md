# Win32_Sample

In the course [Programming 3](https://digitalartsandentertainment.be/page/46/Programming+3) in the
[Game Development](https://digitalartsandentertainment.be/page/31/Game+Development) major at
[**Digital Arts & Entertainment (DAE)**](https://www.digitalartsandentertainment.be/) we were introduced to elementary win32.
The introduction didn't go beyond simple [GDI](https://learn.microsoft.com/en-us/windows/win32/gdi/windows-gdi)
and basic legacy UI elements (menu, button, textbox, label & date picker) and global variables.

I used this project to explore the more advanced "official" [Microsoft path](https://learn.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows).
It serves as my personal repository where I explore what Win32 has to offer while focusing on C++ core guidelines, architectural design and modern C++ in general.

## Technical Scope
* **Modern [OOP](https://learn.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-#an-object-oriented-approach):** Uses a template-based `BaseWindow` class to eliminate global window procedures.
* **[Direct2D](https://learn.microsoft.com/en-us/windows/win32/direct2d/direct2d-portal):** Hardware-accelerated drawing instead of the legacy [GDI](https://learn.microsoft.com/en-us/windows/win32/gdi/windows-gdi) system.
* **[COM](https://learn.microsoft.com/en-us/windows/win32/learnwin32/what-is-a-com-interface-):** Manual lifecycle management of COM interfaces (Factory, RenderTarget) using LIFO cleanup.
* **[DPI](https://learn.microsoft.com/en-us/windows/win32/learnwin32/dpi-and-device-independent-pixels) Awareness:** Handling coordinate conversion between physical pixels and DIPs.
* **C++23:** Leveraging `std::println` and STL algorithms for cleaner logic.

### Project Notes
In this project I use plenty of comments for my future self to speed up onboarding after extended periods of inactivity.
