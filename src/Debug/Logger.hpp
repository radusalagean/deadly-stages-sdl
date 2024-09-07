#ifndef __SRC_DEBUG_LOGGER_HPP__
#define __SRC_DEBUG_LOGGER_HPP__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <cstdlib>

#define logd Logger::logDebug

namespace Logger
{
    void init();
    char* formatString(const char* format, va_list args);
    void logDebug(const char* message, ...);
    void clear();
}

#endif // __SRC_DEBUG_LOGGER_HPP__