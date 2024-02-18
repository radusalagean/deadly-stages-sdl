#ifndef DEADLYSTAGES_LOGGER_H
#define DEADLYSTAGES_LOGGER_H

#include <pspkernel.h>
#include <pspdebug.h>

#define logd Logger::logDebug

namespace Logger
{
    extern bool showingDebugConsole;

    void init();
    char* getWrappedLog(const char* format, ...);
    void logDebug(const char* message, ...);
    void clear();
}

#endif