#ifndef __SAVEDATAHELPER_HPP__
#define __SAVEDATAHELPER_HPP__

#include <string>

namespace SaveDataHelper
{
    extern void saveOptions();
    extern void loadOptions();
    extern void saveHighScores();
    extern void loadHighScores();

    extern void save(const std::string& saveName, const std::string& data, const std::string& title, const std::string& detail = "");
    extern std::string load(const std::string& saveName);
};

#endif // __SAVEDATAHELPER_HPP__
