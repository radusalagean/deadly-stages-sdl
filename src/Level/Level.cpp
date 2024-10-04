#include "Level.hpp"

#include "../Core/Macros.hpp"
#include "../Core/CollisionManager.hpp"

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
    camera.target = &player->position; // The camera will follow the player

    assignWeaponToPlayer(WEAPON_PISTOL);
}

void Level::handleEvents()
{
    // Movement
    if (Game::control.isActionDown(CA_UP) || Game::control.isActionDown(CA_DOWN) || 
        Game::control.isActionDown(CA_LEFT) || Game::control.isActionDown(CA_RIGHT))
    {
        float speedMultiplier = player->speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
        Vector2D velocity;
        velocity.setY(Game::control.isActionDown(CA_UP) ? -speedMultiplier :
                              Game::control.isActionDown(CA_DOWN) ? speedMultiplier : 0);

        velocity.setX(Game::control.isActionDown(CA_LEFT) ? -speedMultiplier :
                              Game::control.isActionDown(CA_RIGHT) ? speedMultiplier : 0);

        CollisionManager::processMovement(*player, velocity, *this);
        player->velocity = velocity;
    }
    else
    {
        player->velocity.setX(0);
        player->velocity.setY(0);
    }

    // Fire
    if (Game::control.isActionDown(CA_FIRE))
    {
        playerWeapon->fire([this](const Vector2D& position, float rotation)
        {
            Bullet* bullet = new Bullet(position, rotation, texturePool);
            bullets.push_back(bullet);
        });
    }
}

void Level::update()
{
    camera.update();
    player->update(camera);
    playerWeapon->update();
    for (auto& bullet : bullets)
    {
        bullet->update(camera);
    }
}

void Level::render()
{
    tileLayer.render(camera);
    player->draw(camera);
    for (auto& bullet : bullets)
    {
        bullet->draw(camera);
    }
}

void Level::dispose()
{
    tileset.clear();

    if (playerWeapon != nullptr)
    {
        delete playerWeapon;
        playerWeapon = nullptr;
    }

    if (player != nullptr)
    {
        delete player;
        player = nullptr;
    }

    for (auto& bullet : bullets)
    {
        delete bullet;
    }
    bullets.clear();
}

SDL_Rect& Level::buildTileRect(int column, int row) const
{
    SDL_Rect tileRect;
    tileRect.x = column * tileWidthPx;
    tileRect.y = row * tileHeightPx;
    tileRect.w = tileWidthPx;
    tileRect.h = tileHeightPx;
    return tileRect;
}

void Level::assignWeaponToPlayer(int weaponId)
{
    if (playerWeapon != nullptr)
    {
        delete playerWeapon;
    }
    playerWeapon = Weapon::createWeapon(weaponId);
    playerWeapon->setOwner(*player);
}

