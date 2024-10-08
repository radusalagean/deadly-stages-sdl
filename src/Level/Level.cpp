#include "Level.hpp"

#include "../Core/Macros.hpp"
#include "../Core/CollisionManager.hpp"
#include "../GameEntity/Enemy.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../Core/VectorUtils.hpp"
#include "../Screen/GameOver/GameOverScreen.hpp"
#include "../ScreenManager/ScreenManager.hpp"

Level::Level(std::string id)
{
    this->id = id;
}

Level::~Level()
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
        velocity.y = Game::control.isActionDown(CA_UP) ? -speedMultiplier :
                              Game::control.isActionDown(CA_DOWN) ? speedMultiplier : 0;

        velocity.x = Game::control.isActionDown(CA_LEFT) ? -speedMultiplier :
                              Game::control.isActionDown(CA_RIGHT) ? speedMultiplier : 0;

        CollisionManager::processMovement(*player, velocity, *this);
        player->velocity = velocity;
    }
    else
    {
        player->velocity.reset();
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
    player->update(*this);
    playerWeapon->update(*this);
    for (auto& bullet : bullets)
    {
        bullet->update(*this);
    }
    for (auto& enemy : enemies)
    {
        enemy->update(*this);
    }
    advanceWaveIfNeeded();
    spawnEnemiesIfNeeded();
    handleGameEntityPendingRemovals();
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
    #ifdef DEBUG_DRAW_COLLISION_RECTS
    renderDebugShapes(camera);
    #endif
}

SDL_Rect Level::buildTileRect(int column, int row) const
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

int Level::getEnemiesLeft()
{
    return enemies.size();
}

#ifdef DEBUG_DRAW_COLLISION_RECTS
void Level::renderDebugShapes(Camera& camera)
{
    // Collided rects: Red
    for (const auto& collidedRect : collidedRects)
    {
        int x = GSCALE(collidedRect.x) - camera.position.x;
        int y = GSCALE(collidedRect.y) - camera.position.y;
        int w = GSCALE(collidedRect.w);
        int h = GSCALE(collidedRect.h);
        SDL_Rect collidedRectDst = { x, y, w, h };
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, collidedRectDst, {255, 0, 0, 255}, 2);
    }
    collidedRects.clear();

    // Check area: Green
    for (const auto& checkAreaRect : checkAreaRects)
    {
        SDL_Rect checkAreaRectDst = 
        {
        static_cast<int>(GSCALE(checkAreaRect.x) - camera.position.x),
        static_cast<int>(GSCALE(checkAreaRect.y) - camera.position.y),
        static_cast<int>(GSCALE(checkAreaRect.w)),
        static_cast<int>(GSCALE(checkAreaRect.h))
        };
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, checkAreaRectDst, {0, 255, 0, 255}, 1);
    }
    checkAreaRects.clear();

    // Tiles in the check area: Yellow
    for (const auto& checkAreaTileIndicesRect : checkAreaTileIndicesRects)
    {
        SDL_Rect checkAreaTileIndicesDst = 
        {
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.x) - camera.position.x),
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.y) - camera.position.y),
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.w)),
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.h))
        };
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, checkAreaTileIndicesDst, {255, 255, 0, 255}, 2);
    }
    checkAreaTileIndicesRects.clear();
}
#endif

void Level::handleGameEntityPendingRemovals()
{
    VectorUtils::removeFromVectorIf(enemies, [this](Enemy* enemy) {
        if (enemy->pendingRemoval)
        {
            if (enemy->bounty > 0)
                score += enemy->bounty;
            delete enemy;
            return true;
        }
        return false;
    });

    VectorUtils::removeFromVectorIf(bullets, [](Bullet* bullet) {
        if (bullet->pendingRemoval)
        {
            delete bullet;
            return true;
        }
        return false;
    });

    if (player->pendingRemoval)
    {
        Game::screenManager.setScreen(new GameOverScreen(score, wave, id));
    }
}
