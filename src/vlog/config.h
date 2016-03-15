/****************************************************************************
 * Copyright (C) 2012-2013 Ievgen Polyvanyi
 * Copyright (C) 2012-2016 Oleg Gushchenkov
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
****************************************************************************/

#pragma once

// Using OpenMP
#define VL_USE_OPENMP

// Using OpenCL
//#define VL_USE_OPENCL

// Build demo version
//#define VL_DEMO_BUILD

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
