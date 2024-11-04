#include "LevelParser.hpp"

#include "Level.hpp"
#include <sstream>
#include "../GameEntity/Player.hpp"
#include "../Core/Macros.hpp"

#define LEVEL_RELATIVE_PATH "res/level/"
#define LEVEL_ABSOLUTE_PATH RPATH(LEVEL_RELATIVE_PATH)

using namespace tinyxml2;

Level* LevelParser::parseLevel(std::string levelId)
{
    XMLDocument levelDocument;
    levelDocument.LoadFile((LEVEL_ABSOLUTE_PATH + levelId + "/map.tmx").c_str());

    Level* level = new Level(levelId);
    XMLElement* root = levelDocument.RootElement();

    level->horizontalTilesCount = root->IntAttribute("width");
    level->verticalTilesCount = root->IntAttribute("height");
    level->tileWidthPx = root->IntAttribute("tilewidth");
    level->tileHeightPx = root->IntAttribute("tileheight");
    level->widthPx = level->horizontalTilesCount * level->tileWidthPx;
    level->heightPx = level->verticalTilesCount * level->tileHeightPx;

    for (XMLElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        const char* value = e->Value();
        if (value == std::string("tileset"))
            parseTileset(levelId, e->Attribute("source"), level->tileset);
        else if (value == std::string("layer"))
            parseTileLayer(e, level->tileLayer, level->tileset);
        else if (value == std::string("objectgroup"))
            parseObjectLayer(e, *level);
    }

    return level;
}

void LevelParser::parseTileset(std::string levelId, const char* tilesetFile, std::map<int, Tile>& tileset)
{
    std::string tilesetPath = std::string(LEVEL_ABSOLUTE_PATH) + levelId + "/" + tilesetFile;
    XMLDocument tilesetDocument;
    tilesetDocument.LoadFile(tilesetPath.c_str());
    XMLElement* tilesRoot = tilesetDocument.RootElement();
    for (XMLElement* rootSubElement = tilesRoot->FirstChildElement(); rootSubElement != nullptr; rootSubElement = rootSubElement->NextSiblingElement())
    {
        const char* value = rootSubElement->Value();
        if (value == std::string("tile"))
        {
            int tileId = rootSubElement->IntAttribute("id");
            std::string path = "";
            int width = 0;
            int height = 0;
            bool collidable = false;
            for (XMLElement* tileSubElement = rootSubElement->FirstChildElement(); tileSubElement != nullptr; tileSubElement = tileSubElement->NextSiblingElement())
            {
                const char* value = tileSubElement->Value();
                if (value == std::string("image"))
                {
                    path = LEVEL_RELATIVE_PATH + levelId + "/" + tileSubElement->Attribute("source");
                    width = tileSubElement->IntAttribute("width");
                    height = tileSubElement->IntAttribute("height");
                }
                else if (value == std::string("properties"))
                {
                    for (XMLElement* propertySubElement = tileSubElement->FirstChildElement(); propertySubElement != nullptr; propertySubElement = propertySubElement->NextSiblingElement())
                    {
                        if (propertySubElement->Value() != std::string("property"))
                            continue;
                        if (propertySubElement->Attribute("name") == std::string("collidable"))
                        {
                            collidable = propertySubElement->BoolAttribute("value");
                        }  
                    }
                }
            }
            tileset.emplace(tileId, Tile(tileId, path, width, height, collidable));
        }
    }
}

void LevelParser::parseTileLayer(XMLElement* layerElement, TileLayer& tileLayer,
    std::map<int, Tile>& tileset)
{
    int width = layerElement->IntAttribute("width");
    int height = layerElement->IntAttribute("height");
    tileLayer.horizontalTilesCount = width;
    tileLayer.verticalTilesCount = height;
    // Parse the tiles from the data element
    std::vector<int> gids(width * height);
    int gidIndex = 0;
    XMLElement* dataElement = layerElement->FirstChildElement("data");
    if (dataElement)
    {
        const char* csvData = dataElement->GetText();
        std::istringstream dataStream(csvData);
        std::string value;

        while (std::getline(dataStream, value, ','))
        {
            if (!value.empty())
            {
                int gid = std::stoi(value);
                gids[gidIndex++] = gid - 1;
            }
        }
    }

    tileLayer.tileMap.resize(height);
    for (int y = 0; y < height; y++)
    {
        tileLayer.tileMap[y].resize(width);
        for (int x = 0; x < width; x++)
        {
            int gid = gids[y * width + x];
            auto it = tileset.find(gid);
            if (it != tileset.end())
            {
                tileLayer.tileMap[y][x] = &(it->second);
            }
            else
            {
                tileLayer.tileMap[y][x] = nullptr;
            }
        }
    }
}

void LevelParser::parseObjectLayer(XMLElement* layerElement, Level& level)
{
    for (XMLElement* e = layerElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        const char* value = e->Value();
        if (value == std::string("object"))
        {
            // Player
            if (e->Attribute("type") == std::string("Player"))
            {
                Player* player = new Player();
                player->setPosition(Vector2D(e->IntAttribute("x"), e->IntAttribute("y")));
                player->setSize(e->IntAttribute("width"), e->IntAttribute("height"));
                level.player = player;
            }
            // SpawnPoint
            else if (e->Attribute("type") == std::string("SpawnPoint"))
            {
                Vector2D* spawnPoint = new Vector2D(e->IntAttribute("x"), e->IntAttribute("y"));
                level.spawnPoints.push_back(spawnPoint);
            }
        }
    }
}