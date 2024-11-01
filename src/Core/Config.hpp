#ifndef __SRC_CORE_CONFIG_HPP__
#define __SRC_CORE_CONFIG_HPP__

#define GAME_NAME "Deadly Stages Remix"

// #define DEBUG_DRAW_COLLISION_RECTS

#if defined(WIN32) || defined(__linux__) || defined(__APPLE__)
    #define PLATFORM_GROUP_COMPUTER
#endif

#if defined(__PSP__)
    #define PLATFORM_GROUP_CONSOLE
    #define PLATFORM_PSP
#endif

#if defined(PLATFORM_PSP)
    #define MINIMUM_WINDOW_WIDTH 480
    #define MINIMUM_WINDOW_HEIGHT 272

    #define SUPPORTS_AIM_ASSIST

    #define MUSIC_FILE_EXT "wav"
#else
    #define MINIMUM_WINDOW_WIDTH 640
    #define MINIMUM_WINDOW_HEIGHT 480

    #define SUPPORTS_SHADOWS
    #define SUPPORTS_MOUSE_POINTER
    #define PRINT_LOGS_TO_FILE

    #define MUSIC_FILE_EXT "ogg"
#endif

#endif // __SRC_CORE_CONFIG_HPP__
