#ifndef CONFIG_H
	#define CONFIG_H

	// These defines could also be set globally in: Project Properties > Preprocessor Definitions
	#define WIN32_LEAN_AND_MEAN // Cuts out the fat (Exclude rarely used APIs)
	#define NOMINMAX            // C++ Compatibility: Stop Windows from breaking std::min and std::max

	#include <windows.h>	// The core
	#include <windowsx.h>	// The "helper" macros
	#include <commctrl.h>
	#include <d2d1.h>		// Direct2D

	// Linker instructions
	#pragma comment(lib, "comctl32.lib")
	#pragma comment(lib, "d2d1")

	// The following line would enable a "modern" look
	#pragma comment(linker,"\"/manifestdependency:type='win32' \
					name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
					processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

	#ifdef _DEBUG
		#include <print>
		#define LOG_PRINT(...) std::println(__VA_ARGS__)
	#else
		#define LOG_PRINT(...) ((void)0) 
	#endif

// TODO 0: Project properties
	// Under General
		// Set C++ standard to C++23 for all builds
	// Under C / C++ > General
		// Set $(ProjectDir)include for all builds
	// Under Linker > System
		// Set Windows(/ SUBSYSTEM:WINDOWS) for Release builds
		// Set Console(/ SUBSYSTEM:CONSOLE) for Debug builds
	// Under Manifest Tool > Input and Output
		// Set Per Monitor High DPI Aware for all builds

#endif
