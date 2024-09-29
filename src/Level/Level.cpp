#include "Level.hpp"

#include "../Core/Macros.hpp"

Level::Level()
{

}

void Level::load()
{
    for (auto& [id, tile] : tileset) 
    {
        tile.load();
    }

    player->load(texturePool);

    camera.mapBounds = new Vector2D(widthPx, heightPx);
    camera.target = player->getPosition(); // The camera will follow the player
}

void Level::handleEvents()
{
    // Movement
    if (Game::controls.isActionDown(CA_UP) || Game::controls.isActionDown(CA_DOWN) || 
        Game::controls.isActionDown(CA_LEFT) || Game::controls.isActionDown(CA_RIGHT))
    {
        float speedMultiplier = player->speedPxPerSeconds * Game::latestLoopDeltaTimeSeconds;
        player->velocity.setY(Game::controls.isActionDown(CA_UP) ? -speedMultiplier :
                              Game::controls.isActionDown(CA_DOWN) ? speedMultiplier : 0);

        player->velocity.setX(Game::controls.isActionDown(CA_LEFT) ? -speedMultiplier :
                              Game::controls.isActionDown(CA_RIGHT) ? speedMultiplier : 0);
    }
    else
    {
        player->velocity.setX(0);
        player->velocity.setY(0);
    }
}

void Level::update()
{
    camera.update();
    player->update();
}

void Level::render()
{
    tileLayer.render(camera);
    player->draw(camera);
}

void Level::dispose()
{
    tileset.clear();
    if (player != nullptr)
    {
        delete player;
        player = nullptr;
    }
}