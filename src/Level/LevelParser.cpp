#include "LevelParser.hpp"

#include "Level.hpp"

#include <sstream>

#define LEVEL_PATH "res/level/"

Level* LevelParser::parseLevel(std::string levelId)
{
    tinyxml2::XMLDocument levelDocument;
    levelDocument.LoadFile((LEVEL_PATH + levelId + "/map.tmx").c_str());

    Level* level = new Level();
    tinyxml2::XMLElement* root = levelDocument.RootElement();

    level->horizontalTilesCount = root->IntAttribute("width");
    level->verticalTilesCount = root->IntAttribute("height");
    level->tileWidthPx = root->IntAttribute("tilewidth");
    level->tileHeightPx = root->IntAttribute("tileheight");
    level->widthPx = level->horizontalTilesCount * level->tileWidthPx;
    level->heightPx = level->verticalTilesCount * level->tileHeightPx;

    for (tinyxml2::XMLElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
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
    std::string tilesetPath = std::string(LEVEL_PATH) + levelId + "/" + tilesetFile;
    tinyxml2::XMLDocument tilesetDocument;
    tilesetDocument.LoadFile(tilesetPath.c_str());
    tinyxml2::XMLElement* tilesRoot = tilesetDocument.RootElement();
    for (tinyxml2::XMLElement* rootSubElement = tilesRoot->FirstChildElement(); rootSubElement != nullptr; rootSubElement = rootSubElement->NextSiblingElement())
    {
        const char* value = rootSubElement->Value();
        if (value == std::string("tile"))
        {
            int tileId = rootSubElement->IntAttribute("id");
            std::string path = "";
            int width = 0;
            int height = 0;
            bool collidable = false;
            for (tinyxml2::XMLElement* tileSubElement = rootSubElement->FirstChildElement(); tileSubElement != nullptr; tileSubElement = tileSubElement->NextSiblingElement())
            {
                const char* value = tileSubElement->Value();
                if (value == std::string("image"))
                {
                    path = LEVEL_PATH + levelId + "/" + tileSubElement->Attribute("source");
                    width = tileSubElement->IntAttribute("width");
                    height = tileSubElement->IntAttribute("height");
                }
                else if (value == std::string("properties"))
                {
                    for (tinyxml2::XMLElement* propertySubElement = tileSubElement->FirstChildElement(); propertySubElement != nullptr; propertySubElement = propertySubElement->NextSiblingElement())
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

void LevelParser::parseTileLayer(tinyxml2::XMLElement* layerElement, TileLayer& tileLayer,
    std::map<int, Tile>& tileset)
{
    int width = layerElement->IntAttribute("width");
    int height = layerElement->IntAttribute("height");
    tileLayer.horizontalTilesCount = width;
    tileLayer.verticalTilesCount = height;
    // Parse the tiles from the data element
    std::vector<int> gids(width * height);
    int gidIndex = 0;
    tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
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

void LevelParser::parseObjectLayer(tinyxml2::XMLElement* layerElement, Level& level)
{
    for (tinyxml2::XMLElement* e = layerElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
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
                player->collisionRect = { // TODO Adjust the collision rect to the player's texture (exclude the black space)
                    0, 
                    0,
                    player->width,
                    player->height
                };
                tinyxml2::XMLElement* properties = e->FirstChildElement("properties");
                for (tinyxml2::XMLElement* property = properties->FirstChildElement(); property != nullptr; property = property->NextSiblingElement())
                {
                    if (property->Attribute("name") == std::string("path"))
                        player->texturePath = property->Attribute("value");
                }
                level.player = player;
            }
        }
    }
}