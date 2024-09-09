#ifndef __SRC_CORE_FONTMANAGER_HPP__
#define __SRC_CORE_FONTMANAGER_HPP__

#include <SDL_ttf.h>
#include <unordered_map>
#include <string>

class FontManager
{
private:
    std::unordered_map<std::string, TTF_Font*> fonts;

public:
    TTF_Font* getFont(const std::string& path, int size);
    void dispose();
};

#endif // __SRC_CORE_FONTMANAGER_HPP__
