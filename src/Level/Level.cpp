#include "Level.hpp"

Level::Level()
{

}

void Level::load()
{
    for (auto& [id, tile] : tileset) {
        tile.load();
    }
}

void Level::update()
{

}

void Level::render(Camera& camera)
{
    tileLayer.render(camera);
}

void Level::dispose()
{

}