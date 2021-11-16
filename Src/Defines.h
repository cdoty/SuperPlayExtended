#pragma once

static const char*	gsc_szAppName		= "SuperPlay";	// App strings
static const char*	gsc_szWindowTitle	= "SuperPlay";
static const int	gsc_iTestWidth		= 1280;			// Test size
static const int	gsc_iTestHeight		= 720;
static const int	gsc_iGameWidth		= 320;			// Window size
static const int	gsc_iGameHeight		= 180;

// Enable AMD and NVidia GPUs on laptops
#define ENABLEGPU extern "C"{__declspec(dllexport) DWORD NvOptimusEnablement = 1; __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;}
