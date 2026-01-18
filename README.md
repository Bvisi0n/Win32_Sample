# Win32_Sample

In the course [Programming 3](https://digitalartsandentertainment.be/page/46/Programming+3) in the
[Game Development](https://digitalartsandentertainment.be/page/31/Game+Development) major at
[**Digital Arts & Entertainment (DAE)**](https://www.digitalartsandentertainment.be/) we were introduced to elementary win32.
The introduction didn't go beyond simple [GDI](https://learn.microsoft.com/en-us/windows/win32/gdi/windows-gdi)
and basic legacy UI elements (menu, button, textbox, label & date picker) and global variables.

I used this project to explore the more advanced "official" [Microsoft path](https://learn.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows).
It serves as my personal repository where I explore what Win32 has to offer while focusing on C++ core guidelines, architectural design and modern C++ in general.

In this project I use plenty of comments for my future self to speed up onboarding after extended periods of inactivity.

## Technical Scope & Features 

### Features
* Pick a background color through the menu bar options.
* Click a button to trigger a popup dialog displaying your entry from a textbox.
* Customize the cursor using radio buttons.
* Manipulate a date picker
* Click a button to open a file select dialog and pick a random file which will display it's name and size in a label.
* Left click anywhere on the background to draw an ellipse.
* Right click on any ellipse to remove it.
* Save and load the ellipses through the menu bar options in both binary and text formats.

### Win32 & Windows API
* Standard Controls & Menus
* [Per-Monitor DPI Awareness](https://learn.microsoft.com/en-us/windows/win32/learnwin32/dpi-and-device-independent-pixels)
* Hardware-accelerated 2D rendering with [Direct2D](https://learn.microsoft.com/en-us/windows/win32/direct2d/direct2d-portal)
* [COM](https://learn.microsoft.com/en-us/windows/win32/learnwin32/what-is-a-com-interface-) resource management using [`Microsoft::WRL::ComPtr`](https://learn.microsoft.com/en-us/cpp/cppcx/wrl/comptr-class?view=msvc-170)
* Shell integration via Common Item Dialog for File Open/Save

### Design Patterns/Choices
* [**Curiously Recurring Template Pattern (CRTP)**](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
  * Implemented in [`BaseWindow<T>`](https://learn.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-#an-object-oriented-approach) to provide a type-safe static [`WindowProc`](https://learn.microsoft.com/en-us/windows/win32/winmsg/window-procedures) that routes OS messages to instance member functions, providing a zero-overhead alternative to virtual dispatch for the static [`WindowProc`](https://learn.microsoft.com/en-us/windows/win32/winmsg/window-procedures) callback.
* [**Resource Acquisition Is Initialization (RAII)**](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r1-manage-resources-automatically-using-resource-handles-and-raii-resource-acquisition-is-initialization)
  * Applied using Smart Pointers such as [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr.html) & [`Microsoft::WRL::ComPtr`](https://learn.microsoft.com/en-us/cpp/cppcx/wrl/comptr-class?view=msvc-170).
  * Encapsulated COM initialization within [`ScopedComContext`](https://github.com/Bvisi0n/Win32_Sample/blob/main/include/ScopedComContext.h).
* **Functional Callback Decoupling** ([Command](https://refactoring.guru/design-patterns/command)/[Observer](https://refactoring.guru/design-patterns/observer))
  * Decoupled UI logic using an Action system where UI events (clicks, text changes) execute functional objects without the controls needing direct knowledge of the window implementation.
* **Conditional Dispatch** ([Strategy](https://refactoring.guru/design-patterns/strategy))
  * Implemented in [`FileService`](https://github.com/Bvisi0n/Win32_Sample/blob/main/include/FileService.h) to dynamically switch between Binary and Regex-based text serialization based on file extensions.
* **Polymorphism**
  * Controls inherit from a pure virtual base class, enabling a unified interface for initialization and layout updates within a single container.
* **Data-Driven Layout Management**
  * UI element ordering and Z-axis depth are managed through the sorted nature of [`std::map`](https://en.cppreference.com/w/cpp/container/map.html) using the enum class control ID as key, managing the order is as easy as changing the values assigned to the controls within the enum class.

### C++ & STL
* Language support library
  * [`<cstdint>`](https://en.cppreference.com/w/cpp/header/cstdint.html)
* Memory management library
  * [`<memory>`](https://en.cppreference.com/w/cpp/header/memory.html)
* Metaprogramming library
  * [`<type_traits>`](https://en.cppreference.com/w/cpp/header/type_traits.html)
* General utilities library
  * [`<functional>`](https://en.cppreference.com/w/cpp/header/functional.html)
  * [`<optional>`](https://en.cppreference.com/w/cpp/header/optional.html)
* Containers library
  * [`<map>`](https://en.cppreference.com/w/cpp/header/map.html)
  * [`<vector>`](https://en.cppreference.com/w/cpp/header/vector.html)
* Ranges library
  * [`<ranges>`](https://en.cppreference.com/w/cpp/header/ranges.html)
* Algorithms library
  * [`<numeric>`](https://en.cppreference.com/w/cpp/header/numeric.html)
  * [`<algorithm>`](https://en.cppreference.com/w/cpp/header/algorithm.html)
* Strings library
  * [`<string>`](https://en.cppreference.com/w/cpp/header/string.html)
  * [`<string_view>`](https://en.cppreference.com/w/cpp/header/string_view.html)
* Text processing library
  * [`<regex>`](https://en.cppreference.com/w/cpp/header/regex.html)
  * [`<format>`](https://en.cppreference.com/w/cpp/header/format.html)
* Time library
  * [`<chrono>`](https://en.cppreference.com/w/cpp/header/chrono.html)
* Input/output library
  * [`<filesystem>`](https://en.cppreference.com/w/cpp/header/filesystem.html)
  * [`<fstream>`](https://en.cppreference.com/w/cpp/header/fstream.html)
  * [`<iostream>`](https://en.cppreference.com/w/cpp/header/iostream.html)

**Note**: Concepts were implemented to guard the binary read/write templates.
