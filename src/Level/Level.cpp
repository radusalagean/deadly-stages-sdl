#include "Level.hpp"

#include "../Core/Macros.hpp"
#include "../Core/CollisionManager.hpp"

Level::Level(std::string id)
{
    this->id = id;
}

void Level::load()
{
    for (auto& [id, tile] : tileset) 
    {
        tile.load();
    }

    player->load(texturePool);

    camera.mapBounds = new Vector2D(widthPx, heightPx);
    camera.target = &player->positionPlusCenter; // The camera will follow the player

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

            // Debug: Kill enemy
            if (!enemies.empty())
            {
                auto enemy = enemies[0];
                delete enemy;
                enemies.erase(enemies.begin());
                logd("Enemies left: %d", enemies.size());
            }
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
    for (auto& enemy : enemies)
    {
        enemy->update(camera);
    }
    advanceWaveIfNeeded();
    spawnEnemiesIfNeeded();
}

void Level::render()
{
    tileLayer.render(camera);
    player->draw(camera);
    for (auto& bullet : bullets)
    {
        bullet->draw(camera);
    }
    for (auto& enemy : enemies)
    {
        enemy->draw(camera);
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

    for (auto& enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
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

void Level::advanceWaveIfNeeded()
{
    if (enemiesLeftToSpawn > 0 || !enemies.empty())
        return;
    wave++;
    enemiesLeftToSpawn = wave * 6;
    currentEnemySpeedMultiplier *= 1.05f;
    logd("New wave: %d, enemies left to spawn: %d", wave, enemiesLeftToSpawn);
}

void Level::spawnEnemiesIfNeeded()
{
    if (enemiesLeftToSpawn <= 0)
        return;
    if (spawnEnemyDebouncer.canPerformAction())
    {
        for (auto& spawnPoint : spawnPoints)
        {
            if (enemiesLeftToSpawn <= 0)
                break;

            Enemy* enemy = new Enemy();
            enemy->setPosition(*spawnPoint);
            enemy->setSize(32, 32);
            enemy->texturePath = "res/level/" + id + "/enemy.png";
            enemy->target = &player->positionPlusCenter;
            enemy->speedPxPerSecond *= currentEnemySpeedMultiplier;
            enemies.push_back(enemy);
            enemy->load(texturePool);
            enemiesLeftToSpawn--;
        }
    }
}
