#ifndef __SRC_PLATFORM_COMMON_SAVEDATAHELPER_HPP__
#define __SRC_PLATFORM_COMMON_SAVEDATAHELPER_HPP__

#include <string>

namespace SaveDataHelper
{
    extern void saveOptions();
    extern void loadOptions();
    extern void saveHighScores();
    extern void loadHighScores();

    extern void save(const std::string& filename, const std::string& data);
    extern std::string load(const std::string& filename);
};

#endif // __SRC_PLATFORM_COMMON_SAVEDATAHELPER_HPP__
