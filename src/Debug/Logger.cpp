#include "Logger.hpp"

#include <stdio.h>
#include <SDL.h>

namespace Logger
{
    void init()
    {
        // No-Op
    }

    char* formatString(const char* format, va_list args) {
        char result[1024];  // Adjust the buffer size as needed
        int pos = 0;

        while (*format != '\0') {
            if (*format == '%') {
                ++format; // Move past '%'

                if (*format == 'd') {
                    int arg = va_arg(args, int);
                    pos += snprintf(result + pos, sizeof(result) - pos, "%d", arg);
                } else if (*format == 'f') {
                    double arg = va_arg(args, double);
                    pos += snprintf(result + pos, sizeof(result) - pos, "%f", arg);
                } else if (*format == 's') {
                    const char* arg = va_arg(args, const char*);
                    pos += snprintf(result + pos, sizeof(result) - pos, "%s", arg);
                } else {
                    // Unsupported placeholder, just append it
                    result[pos++] = '%';
                    result[pos++] = *format;
                }
            } else {
                result[pos++] = *format;
            }

            ++format;
        }

        result[pos++] = '\n';
        result[pos] = '\0';
        return strdup(result);  // Requires #include <string.h> for strdup
    }

    void logDebug(const char* format, ...)
    {
        va_list args;
        va_start(args, format);

        char* result = formatString(format, args);

        va_end(args);

        printf("%s", result);
#ifndef __PSP__
        FILE* pFile;
        pFile = fopen("Log.txt", "a");
        fwrite(result, strlen(result), 1, pFile);
        fclose(pFile);
#endif
        free(result);
    }

    void logSDLError()
    {
        logDebug("SDL Error: %s", SDL_GetError());
    }

    void clear()
    {
        // No-Op
    }
}