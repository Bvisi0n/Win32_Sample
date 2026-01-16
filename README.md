# Win32_Sample

In the course [Programming 3](https://digitalartsandentertainment.be/page/46/Programming+3) in the
[Game Development](https://digitalartsandentertainment.be/page/31/Game+Development) major at
[**Digital Arts & Entertainment (DAE)**](https://www.digitalartsandentertainment.be/) we were introduced to elementary win32.
The introduction didn't go beyond simple [GDI](https://learn.microsoft.com/en-us/windows/win32/gdi/windows-gdi)
and basic legacy UI elements (menu, button, textbox, label & date picker) and global variables.

I used this project to explore the more advanced "official" [Microsoft path](https://learn.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows).
It serves as my personal repository where I explore what Win32 has to offer while focusing on C++ core guidelines, architectural design and modern C++ in general.

## Technical Scope
* **[Template-Based Windowing (CRTP)](https://learn.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-#an-object-oriented-approach):** Wrapped the Win32 API using the [CRTP](https://learn.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-#an-object-oriented-approach) (`BaseWindow<T>`). This eliminates global state by using `SetWindowLongPtr` to store the `this` pointer, mapping static OS callbacks directly to class member functions.
* **Polymorphic UI Framework:** Architected a UI system around a base `Control` class to encapsulate standard Win32 handles.
  * **Container Design:** Managed UI elements within a `std::map<UI::ControlID, std::unique_ptr<Control>>`.
  * **Automated Tab/Z-Order:** Leveraged the sorted nature of the `std::map` and a strictly ordered `enum class ControlID` to define the initialization sequence. This naturally handles the Tab-index and Z-order without manual position management.
* **Decoupled UI Logic:** Implemented a clean separation of concerns by routing `WM_COMMAND` and `WM_NOTIFY` events through an Action-based system. This keeps the `MainWindow` logic focused on orchestration rather than individual control behavior.
* **Hardware-Accelerated Rendering:** Integrated [Direct2D](https://learn.microsoft.com/en-us/windows/win32/direct2d/direct2d-portal) to handle dynamic mouse-input rendering (ellipses). Managed the graphics pipeline and resource lifecycles using [COM](https://learn.microsoft.com/en-us/windows/win32/learnwin32/what-is-a-com-interface-) (`ComPtr`) and a custom `ScopedComContext` for RAII-based initialization.
* **FileService & Persistence:** Developed a flexible I/O system using `std::filesystem` and `fstream`.
  * **Binary:** Type-safe serialization enforced via C++20 Concepts.
  * **Text:** Utilized Regex for robust parsing of external configuration files, enabling "data-driven" tweaks (like UI constants) without recompiling the executable.
* **High-Resolution Benchmarking:** Built a custom profiling utility using `std::chrono` to measure engine-level operations with nanosecond precision, featuring statistical outlier trimming.
* **Modern C++ Standards:** Leveraged C++20/23 features, specifically `std::concepts` for template constraints and Type Traits to ensure compile-time safety across the codebase.
* **[Per-Monitor DPI Awareness](https://learn.microsoft.com/en-us/windows/win32/learnwin32/dpi-and-device-independent-pixels):** Implemented high-DPI scaling logic to calculate Device Independent Pixels (DIPs), ensuring rendering remains consistent across mixed-resolution display setups.

### Project Notes
In this project I use plenty of comments for my future self to speed up onboarding after extended periods of inactivity.
