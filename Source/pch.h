#pragma once

///
/// C Standard Library (used by ImGui and its dependencies)
/// 

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <float.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <immintrin.h>


///
/// C++ Standard Template Library
/// 

#include <cstdint>
#include <iostream>
#include <bitset>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <set>
#include <map>
#include <list>
#include <regex>
#include <array>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cfloat>
#include <utility>
#include <functional>
#include <unordered_map>

///
/// Windows API (used by an ImGui dependency)
/// 
#if WIN32 || _WIN32
    #include <winsdkver.h>
    #define _WIN32_WINNT 0x0A00
    #include <sdkddkver.h>
    
    // Use the C++ standard templated min/max
    #define NOMINMAX
    
    // We don't need GDI
    #define NODRAWTEXT
    #define NOGDI
    #define NOBITMAP
    
    // Include <mcx.h> if you need this
    #define NOMCX
    
    // Include <winsvc.h> if you need this
    #define NOSERVICE
    
    // WinHelp is deprecated
    #define NOHELP
    
    // Exclude extra headers that aren't commonly needed
    #define WIN32_LEAN_AND_MEAN

    // ImGui defines this in several places
    #if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
        #define _CRT_SECURE_NO_WARNINGS
    #endif
    
    #include <Windows.h>
    #include <intrin.h> // Used by 'EMULATOR_DEBUG_BREAK()' for the '__nop()' instruction
#endif


///
/// SDL2
/// 

#include <SDL.h>