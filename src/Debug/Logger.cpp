#include "Logger.hpp"

namespace Logger
{
    bool showingDebugConsole = false;

    void init()
    {
        pspDebugScreenInit();
    }

    void logDebug(const char* format, ...)
    {
        pspDebugScreenPrintf(format);
    }

    void clear()
    {
        pspDebugScreenClear();
    }
}