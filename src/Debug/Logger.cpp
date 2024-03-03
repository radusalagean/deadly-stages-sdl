#include "Logger.hpp"

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
                    pos += sprintf(result + pos, "%d", arg);
                } else if (*format == 'f') {
                    double arg = va_arg(args, double);
                    pos += sprintf(result + pos, "%f", arg);
                } else if (*format == 's') {
                    const char* arg = va_arg(args, const char*);
                    pos += sprintf(result + pos, "%s", arg);
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

        unsigned int length;
        FILE* pFile;
        length = strlen(result);
        pFile = fopen("PSP_Log.txt", "a");
        fwrite(result, strlen(result), 1, pFile);
        fclose(pFile);

        free(result);
    }

    void clear()
    {
        // No-Op
    }
}