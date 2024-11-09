#include "SaveDataHelper.hpp"

#include <string>
#include <cstdint>
#include "../../Adapter/OptionsAdapter.hpp"
#include "../../Adapter/HighScoresAdapter.hpp"
#include "../../Adapter/HighScoresModel.hpp"
#include "../../Game.hpp"
#include "../../Core/Config.hpp"

void platformSaveOptions()
{
    SaveDataHelper::saveOptions();
}

void platformLoadOptions()
{
    SaveDataHelper::loadOptions();
}

void platformSaveHighScores()
{
    SaveDataHelper::saveHighScores();
}

void platformLoadHighScores()
{
    SaveDataHelper::loadHighScores();
}

namespace SaveDataHelper
{
    void saveOptions()
    {
        OptionsModel options = OptionsAdapter::extractOptionsFromGame();
        std::string optionsXml = OptionsAdapter::serializeOptions(options);
        
        save("OPTIONS.BIN", optionsXml);
    }

    void loadOptions() 
    {
        std::string loadedData = load("OPTIONS.BIN");

        OptionsModel options = OptionsAdapter::deserializeOptions(loadedData);
        OptionsAdapter::applyOptionsFromModel(options);
    }

    void saveHighScores()
    {
        HighScoresModel highScores = Game::highScores;
        std::string highScoresXml = HighScoresAdapter::serializeHighScores(highScores);

        save("HIGH_SCORES.BIN", highScoresXml);
    }

    void loadHighScores()
    {
        std::string loadedData = load("HIGH_SCORES.BIN");

        HighScoresModel highScores = HighScoresAdapter::deserializeHighScores(loadedData);
        Game::highScores = highScores;
    }

    void save(const std::string& filename, const std::string& data)
    {
        // Calculate checksum
        uint32_t checksum = 0;
        for (char c : data) 
        {
            checksum = ((checksum << 5) + checksum) + c;
        }

        // Encrypt data with XOR cipher
        const char key[] = SAVE_FILE_ENCRYPTION_KEY;
        std::string encrypted = data;
        for (size_t i = 0; i < encrypted.length(); i++) 
        {
            encrypted[i] ^= key[i % sizeof(key)];
        }

        // Write encrypted data and checksum to file
        FILE* file = fopen(filename.c_str(), "wb");
        if (file) 
        {
            fwrite(&checksum, sizeof(checksum), 1, file);
            fwrite(encrypted.c_str(), encrypted.length(), 1, file);
            fclose(file);
        }
    }

    std::string load(const std::string& filename)
    {
        std::string loadedData;
        FILE* file = fopen(filename.c_str(), "rb");
        if (file) 
        {
            // Read checksum
            uint32_t storedChecksum;
            fread(&storedChecksum, sizeof(storedChecksum), 1, file);

            // Read encrypted data
            fseek(file, 0, SEEK_END);
            long fileSize = ftell(file) - sizeof(uint32_t);
            fseek(file, sizeof(uint32_t), SEEK_SET);

            std::string encrypted;
            encrypted.resize(fileSize);
            fread(&encrypted[0], fileSize, 1, file);
            fclose(file);

            // Decrypt data
            const char key[] = SAVE_FILE_ENCRYPTION_KEY;
            std::string decrypted = encrypted;
            for (size_t i = 0; i < decrypted.length(); i++) 
            {
                decrypted[i] ^= key[i % sizeof(key)];
            }

            // Verify checksum
            uint32_t checksum = 0;
            for (char c : decrypted)
            {
                checksum = ((checksum << 5) + checksum) + c;
            }

            if (checksum != storedChecksum) 
            {
                // printf("File %s checksum mismatch\n", filename.c_str());
            }
            else
            {
                // printf("Loaded %s:\n%s\n\n\n", filename.c_str(), decrypted.c_str());
                loadedData = decrypted;
            }
        }

        return loadedData;
    }
};
