#ifndef __SRC_ADAPTER_HIGHSCORESADAPTER_HPP__
#define __SRC_ADAPTER_HIGHSCORESADAPTER_HPP__

#include "HighScoresModel.hpp"

namespace HighScoresAdapter
{
    extern std::string serializeHighScores(HighScoresModel& highScores);
    extern HighScoresModel deserializeHighScores(std::string& serializedHighScores);
};

#endif // __SRC_ADAPTER_HIGHSCORESADAPTER_HPP__
