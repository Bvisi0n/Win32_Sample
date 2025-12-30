#ifndef CONFIG_H
#define CONFIG_H

// These defines could also be set globally in: Project Properties > Preprocessor Definitions
#define UNICODE             // Affects how text is interpreted
#define WIN32_LEAN_AND_MEAN // Cuts out the fat (Exclude rarely used APIs)
#define NOMINMAX            // C++ Compatibility: Stop Windows from breaking std::min and std::max

#include <d2d1.h> // Also has a min macro!
#include <windows.h>

// Linker instructions
#pragma comment(lib, "d2d1")

#endif
