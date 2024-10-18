#ifndef __SRC_CORE_CONFIG_HPP__
#define __SRC_CORE_CONFIG_HPP__

// #define DEBUG_DRAW_COLLISION_RECTS

#if defined(WIN32) || defined(__linux__) || defined(__APPLE__)
    #define PLATFORM_GROUP_COMPUTER
    #define SUPPORTS_MOUSE_POINTER
    #define PRINT_LOGS_TO_FILE
#endif

#if defined(__PSP__)
    #define PLATFORM_GROUP_CONSOLE
    #define PLATFORM_PSP
    #define SUPPORTS_AUTOAIM
#endif

#endif // __SRC_CORE_CONFIG_HPP__
