#include "HighScoresAdapter.hpp"

#include <tinyxml2.h>

using namespace tinyxml2;

namespace HighScoresAdapter
{
    static constexpr const char* ROOT_HIGH_SCORES = "high_scores";
    static constexpr const char* ROOT_HIGH_SCORES_ATTR_SCHEMA_VERSION = "schema_version";
    static constexpr const int CURRENT_SCHEMA_VERSION = 1;

    static constexpr const char* PROPERTY_LEVEL_HIGH_SCORE = "level_high_score";
    static constexpr const char* PROPERTY_LEVEL_ID = "level_id";

    std::string serializeHighScores(HighScoresModel& highScores)
    {
        XMLPrinter printer;
        printer.OpenElement(ROOT_HIGH_SCORES);
        printer.PushAttribute(ROOT_HIGH_SCORES_ATTR_SCHEMA_VERSION, CURRENT_SCHEMA_VERSION);
        for (auto& [levelId, entry] : highScores.entries)
        {
            printer.OpenElement(PROPERTY_LEVEL_HIGH_SCORE);
            printer.PushAttribute(PROPERTY_LEVEL_ID, levelId.c_str());
            printer.PushText(entry.score);
            printer.CloseElement();
        }
        printer.CloseElement();
        const char* printerText = printer.CStr();
        return std::string(printerText);
    }

    HighScoresModel deserializeHighScores(std::string& serializedHighScores)
    {
        HighScoresModel highScores;

        XMLDocument doc;
        doc.Parse(serializedHighScores.c_str());
        if (doc.Error())
        {
            printf("Error parsing high scores: %s\n", doc.ErrorStr());
            return highScores;
        }

        XMLElement* root = doc.RootElement();

        for (XMLElement* levelHighScore = root->FirstChildElement(PROPERTY_LEVEL_HIGH_SCORE); 
            levelHighScore != nullptr; 
            levelHighScore = levelHighScore->NextSiblingElement(PROPERTY_LEVEL_HIGH_SCORE))
        {
            HighScoreEntryModel entry;
            entry.levelId = levelHighScore->Attribute(PROPERTY_LEVEL_ID);
            entry.score = levelHighScore->IntText();
            highScores.entries[entry.levelId] = entry;
        }

        return highScores;
    }
}
