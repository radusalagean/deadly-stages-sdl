#ifndef __SRC_CORE_CONFIG_HPP__
#define __SRC_CORE_CONFIG_HPP__

// #define DEBUG_DRAW_COLLISION_RECTS

#if defined(WIN32) || defined(__linux__) || defined(__APPLE__)
    #define SUPPORTS_MOUSE_POINTER
    #define PRINT_LOGS_TO_FILE
#endif


#endif // __SRC_CORE_CONFIG_HPP__
