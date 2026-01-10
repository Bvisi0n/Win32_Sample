#ifndef CONFIG_H
	#define CONFIG_H

	//#define WIN32_LEAN_AND_MEAN
	//#define NOMINMAX
	//#pragma comment(lib, "comctl32.lib")
	//#pragma comment(lib, "d2d1")
	//#pragma comment(linker,"\"/manifestdependency:type='win32' \
	//					name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	//					processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

	#ifdef _DEBUG
		#include <print>
		#define LOG_PRINT(...) std::println(__VA_ARGS__)
	#else
		#define LOG_PRINT(...) ((void)0) 
	#endif

// TODO 0: Project properties
	// Under General
		// Set C++ standard to C++23 for all builds
	// Under C/C++ > General
		// Set $(ProjectDir)include for all builds
	// Under C/C++ > Preprocessor
		// Set Preprocessor Definitions to
			// NDEBUG;_WINDOWS;WIN32_LEAN_AND_MEAN;NOMINMAX;%(PreprocessorDefinitions)
			// For Release builds
		// Set Preprocessor Definitions to
			// _DEBUG;_CONSOLE;WIN32_LEAN_AND_MEAN;NOMINMAX;%(PreprocessorDefinitions)
			// for Debug builds
	// Under Linker > Input
		// Set Additional Dependencies to
			// d2d1.lib;comctl32.lib;%(AdditionalDependencies)
			// for all builds
	// Under Linker > Manifest File (for a "modern" UI look)
		// set Additional Manifest Dependencies to
			// type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'
			// for all builds
	// Under Linker > System
		// Set Windows(/ SUBSYSTEM:WINDOWS) for Release builds
		// Set Console(/ SUBSYSTEM:CONSOLE) for Debug builds
	// Under Manifest Tool > Input and Output
		// Set Per Monitor High DPI Aware for all builds

// It's extremely difficult to find anything on the definitions _CONSOLE and _WINDOWS
// The only source I can find is: https://help.perforce.com/sourcepro/legacy-hpp/genbd/4-2.html
// I don't think it's even being used in this setup...
// TODO Research: Experiment with _CONSOLE and _WINDOWS

#endif
