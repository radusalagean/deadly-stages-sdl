#ifndef __SRC_ADAPTER_HIGHSCORESMODEL_HPP__
#define __SRC_ADAPTER_HIGHSCORESMODEL_HPP__

#include <string>
#include <unordered_map>

struct HighScoreEntryModel
{
    std::string levelId = "";
    int score = 0;
};

struct HighScoresModel
{
    std::unordered_map<std::string, HighScoreEntryModel> entries;
    bool pendingSave = false;

    int getScore(const std::string& levelId) const
    {
        if (entries.find(levelId) != entries.end())
        {
            return entries.at(levelId).score;
        }
        return 0;
    }
};

#endif // __SRC_ADAPTER_HIGHSCORESMODEL_HPP__
