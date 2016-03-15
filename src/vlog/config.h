#pragma once

// Using OpenMP
#define VL_USE_OPENMP

// Using OpenCL
//#define VL_USE_OPENCL

// Build demo version
#define VL_DEMO_BUILD

// OS
#if defined(__WIN32__) || defined(_WIN32)
#	define VL_OS_WINDOWS
#else
#	define VL_OS_LINUX
#endif

// Platform
#if defined(__x86_64__) || defined(_M_X64)
#	define VL_PLATFORM_64
#else
#	define VL_PLATFORM_32
#endif

// Pointer size
#ifdef VL_PLATFORM_64
	typedef unsigned long long int t_pointer;
#else
	typedef unsigned int t_pointer;
#endif

// Copy memory function
#ifdef VL_OS_LINUX
#	define memcpy_s(dest, dest_size, src, src_size) memcpy((dest), (src), (src_size))
#endif
