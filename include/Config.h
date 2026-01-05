#ifndef CONFIG_H
	#define CONFIG_H

	// These defines could also be set globally in: Project Properties > Preprocessor Definitions
	#define UNICODE             // Affects how text is interpreted
	#define WIN32_LEAN_AND_MEAN // Cuts out the fat (Exclude rarely used APIs)
	#define NOMINMAX            // C++ Compatibility: Stop Windows from breaking std::min and std::max

	#include <windows.h>	// The core
	#include <windowsx.h>	// The "helper" macros
	#include <d2d1.h>		// Direct2D

	// Linker instructions
	#pragma comment(lib, "d2d1")

	#ifdef _DEBUG
		#include <print>
		#define LOG_PRINT(...) std::println(__VA_ARGS__)
	#else
		#define LOG_PRINT(...) ((void)0) 
	#endif

// TODO!: Project properties
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
