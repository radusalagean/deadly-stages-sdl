#ifndef DEADLYSTAGES_LOGGER_H
#define DEADLYSTAGES_LOGGER_H

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

#endif