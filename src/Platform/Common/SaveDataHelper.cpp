#include "SaveDataHelper.hpp"

#include <string>
#include "../../Adapter/OptionsAdapter.hpp"
#include "../../Core/Config.hpp"
#include "../../Debug/Logger.hpp"

void platformSaveOptions()
{
    SaveDataHelper::saveOptions();
}

void platformLoadOptions()
{
    SaveDataHelper::loadOptions();
}

namespace SaveDataHelper
{
    void saveOptions()
    {
        OptionsModel options = OptionsAdapter::extractOptionsFromGame();
        std::string optionsXml = OptionsAdapter::serializeOptions(options);
        
        // Calculate checksum
        uint32_t checksum = 0;
        for (char c : optionsXml) 
        {
            checksum = ((checksum << 5) + checksum) + c;
        }

        // Encrypt data with XOR cipher
        const char key[] = SAVE_FILE_ENCRYPTION_KEY;
        std::string encrypted = optionsXml;
        for (size_t i = 0; i < encrypted.length(); i++) 
        {
            encrypted[i] ^= key[i % sizeof(key)];
        }

        // Write encrypted data and checksum to file
        FILE* file = fopen("OPTIONS.BIN", "wb");
        if (file) 
        {
            fwrite(&checksum, sizeof(checksum), 1, file);
            fwrite(encrypted.c_str(), encrypted.length(), 1, file);
            fclose(file);
        }
    }

    void loadOptions() 
    {
        std::string loadedData;
        FILE* file = fopen("OPTIONS.BIN", "rb");
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
                logd("Options file checksum mismatch");
            }
            else
            {
                logd("Loaded options:\n%s\n\n", decrypted.c_str());
                loadedData = decrypted;
            }
        }

        // Parse and apply options
        OptionsModel options = OptionsAdapter::deserializeOptions(loadedData);
        OptionsAdapter::applyOptionsFromModel(options);
    }
};
