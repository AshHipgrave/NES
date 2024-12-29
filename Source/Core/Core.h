#pragma once

///
/// Platform detection
/// 
#ifdef _WIN32
    #define PLATFORM_WINDOWS 1
#else
    #error Unknown platform. Please add support before compiling the project.
#endif

///
/// Compiler detection
/// 
#if defined(__clang__)
    #define COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define COMPILER_GCC 1
#elif defined(_MSC_VER)
    #define COMPILER_MSVC 1
#else
    #error Unknown or unsupported compiler. Please add support before compiling the project.
#endif

///
/// Build Configuration
///
#if defined(DEBUG) || defined(_DEBUG)
    #define EMULATOR_DEBUG_BUILD 1
#elif defined(NDEBUG) || defined(_NDEBUG)
    #define EMULATOR_RELEASE_BUILD 1
#else
    #error Unknown build configuration. Please define DEBUG (or _DEBUG) for debug configurations and NDEBUG (or _NDEBUG) for Release configurations.
#endif

///
/// Debug break definition
/// 
#if EMULATOR_DEBUG_BUILD
    #if COMPILER_MSVC
        // __nop() is used to work around a VS bug where it can mistakenly skip over a breakpoint when single-stepping through code
        #define EMULATOR_DEBUG_BREAK() (__nop(), __debugbreak())
    #elif COMPILER_CLANG || COMPILER_GCC
        #define EMULATOR_DEBUG_BREAK() asm("int $3")
    #endif
#else
    #define EMULATOR_DEBUG_BREAK() ((void)0)
#endif

///
/// Unused parameter definition to prevent compiler warnings appearing as errors
/// 
#define UNUSED_PARAMETER(P) (P)