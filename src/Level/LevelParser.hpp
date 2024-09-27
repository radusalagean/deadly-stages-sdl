#ifndef __SRC_LEVEL_LEVELPARSER_HPP_
#define __SRC_LEVEL_LEVELPARSER_HPP_

#include <tinyxml2.h>
#include <string>
#include <vector>
#include <map>

#include "Tile.hpp"
#include "TileLayer.hpp"

class Level;

class LevelParser
{
public:
    Level* parseLevel(std::string levelId);

private:
    void parseTileset(std::string levelId, const char* tilesetFile, std::map<int, Tile>& tileset);
    void parseTileLayer(tinyxml2::XMLElement* layerElement, TileLayer& tileLayer,
        std::map<int, Tile>& tileset);
    void parseObjectLayer(tinyxml2::XMLElement* layerElement, Level& level);
};

#endif // __SRC_LEVEL_LEVELPARSER_HPP_
