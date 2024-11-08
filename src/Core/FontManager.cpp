#include "FontManager.hpp"

#include <string>
#include <iostream>
#include "Macros.hpp"

FontManager::~FontManager() {
    for (auto& pair : fonts) {
        TTF_CloseFont(pair.second);
    }
    fonts.clear();
    TTF_Quit();
}

TTF_Font* FontManager::getFont(const std::string& fontFileName, int size) {
    std::string key = fontFileName + std::to_string(size);
    if (fonts.find(key) == fonts.end()) 
    {
        std::string path = RPATH("res/font/" + fontFileName);
        fonts[key] = TTF_OpenFont(path.c_str(), size);
        if (!fonts[key]) 
        {
            printf("Failed to load font: %s with size %d. TTF_GetError(): %s\n", fontFileName.c_str(), size, TTF_GetError());
        }
    }
    return fonts[key];
}