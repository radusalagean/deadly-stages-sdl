#include "Level.hpp"

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
    if (Game::controls.isActionDown(CA_UP))
    {
        player->velocity.setY(-player->speedPxPerSeconds * Game::latestLoopDeltaTimeSeconds);
    }
    else if (Game::controls.isActionDown(CA_DOWN))
    {
        player->velocity.setY(player->speedPxPerSeconds * Game::latestLoopDeltaTimeSeconds);
    }
    else
    {
        player->velocity.setY(0);
    }

    if (Game::controls.isActionDown(CA_LEFT))
    {
        player->velocity.setX(-player->speedPxPerSeconds * Game::latestLoopDeltaTimeSeconds);
    }
    else if (Game::controls.isActionDown(CA_RIGHT))
    {
        player->velocity.setX(player->speedPxPerSeconds * Game::latestLoopDeltaTimeSeconds);
    }
    else
    {
        player->velocity.setX(0);
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