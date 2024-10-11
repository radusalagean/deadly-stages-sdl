#include "Level.hpp"

#include "../Core/Macros.hpp"
#include "../Core/CollisionManager.hpp"
#include "../GameEntity/Enemy.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../Core/VectorUtils.hpp"
#include "../Screen/GameOver/GameOverScreen.hpp"
#include "../ScreenManager/ScreenManager.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"

Level::Level(std::string id)
{
    this->id = id;
}

Level::~Level()
{
    tileset.clear();

    for (auto& weapon : playerWeapons)
    {
        delete weapon;
    }
    playerWeapons.clear();

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

    onWeaponOrMagReceived(WeaponId::WEAPON_PISTOL);
}

void Level::handleEvents()
{
    // Movement
    if (Game::control.isActionDown(CA_UP) || Game::control.isActionDown(CA_DOWN) || 
        Game::control.isActionDown(CA_LEFT) || Game::control.isActionDown(CA_RIGHT))
    {
        float speed = player->speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
        if (Game::control.isActionDown(CA_SPRINT) && player->stamina > 0.0f)
        {
            speed += player->sprintBoost * Game::latestLoopDeltaTimeSeconds;
            player->stamina -= player->staminaDecreaseRatePerSecond * Game::latestLoopDeltaTimeSeconds;
            if (player->stamina < 0.0f)
                player->stamina = 0.0f;
            player->lastStaminaDecreaseTime = std::chrono::steady_clock::now();
        }
        Vector2D velocity;
        velocity.y = Game::control.isActionDown(CA_UP) ? -speed :
                              Game::control.isActionDown(CA_DOWN) ? speed : 0;

        velocity.x = Game::control.isActionDown(CA_LEFT) ? -speed :
                              Game::control.isActionDown(CA_RIGHT) ? speed : 0;

        CollisionManager::processMovement(*player, velocity, *this, nullptr, player->isJumping());
        player->velocity = velocity;
    }
    else
    {
        player->velocity.reset();
    }

    // Jump
    if (Game::control.isActionDown(CA_JUMP))
    {
        player->onJumpRequest();
        Game::control.releaseAndBlockAction(CA_JUMP);
    }

    // Fire
    if (Game::control.isActionDown(CA_FIRE))
    {
        currentPlayerWeapon->onFireRequest([this](const Vector2D& position, float rotation)
        {
            Bullet* bullet = new Bullet(position, rotation, texturePool);
            bullets.push_back(bullet);
        });
    }

    // Reload
    if (Game::control.isActionDown(CA_RELOAD))
    {
        currentPlayerWeapon->reloadIfPossible();
    }

    // Cycle weapon
    if (Game::control.isActionDown(CA_PREVIOUS_WEAPON))
    {
        cycleWeapon(-1);
        Game::control.releaseAction(CA_PREVIOUS_WEAPON);
    }
    else if (Game::control.isActionDown(CA_NEXT_WEAPON))
    {
        cycleWeapon(1);
        Game::control.releaseAction(CA_NEXT_WEAPON);
    }

    // Debug weapons
    // TODO - Later, change this to it only selects the weapon from the player's inventory, after pickups are implemented
    if (Game::control.isActionDown(CA_DEBUG_WEAPON_1))
    {
        onWeaponOrMagReceived(WeaponId::WEAPON_PISTOL);
        Game::control.releaseAndBlockAction(CA_DEBUG_WEAPON_1);
    }
    if (Game::control.isActionDown(CA_DEBUG_WEAPON_2))
    {
        onWeaponOrMagReceived(WeaponId::WEAPON_SHOTGUN);
        Game::control.releaseAndBlockAction(CA_DEBUG_WEAPON_2);
    }
    if (Game::control.isActionDown(CA_DEBUG_WEAPON_3))
    {
        onWeaponOrMagReceived(WeaponId::WEAPON_SMG);
        Game::control.releaseAndBlockAction(CA_DEBUG_WEAPON_3);
    }
}

void Level::update()
{
    camera.update();
    player->update(*this);
    currentPlayerWeapon->update(*this);
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
    for (auto& bullet : bullets)
    {
        bullet->draw(camera);
    }
    for (auto& enemy : enemies)
    {
        enemy->draw(camera);
    }
    
    player->draw(camera);
    
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

void Level::onWeaponOrMagReceived(WeaponId weaponId)
{
    auto found = std::find_if(playerWeapons.begin(), playerWeapons.end(), [weaponId](const Weapon* weapon) {
        return weapon->id == weaponId;
    });

    if (found == playerWeapons.end()) // Weapon not found in the player's inventory
    {
        Weapon* newWeapon = Weapon::createWeapon(weaponId, texturePool);
        playerWeapons.push_back(newWeapon);
        currentPlayerWeapon = newWeapon;
        currentPlayerWeapon->setOwner(*player);
    }
    else
    {
        currentPlayerWeapon = *found;
    }
    if (!currentPlayerWeapon->hasInfiniteMags)
        currentPlayerWeapon->availableMags += wave;
}

void Level::cycleWeapon(int offset)
{
    if (playerWeapons.size() <= 1 || offset == 0)
        return;
    if (offset < -1) offset = -1;
    if (offset > 1) offset = 1;
    auto it = std::find(playerWeapons.begin(), playerWeapons.end(), currentPlayerWeapon);
    if (it != playerWeapons.end()) {
        it += offset;
        if (it == playerWeapons.end()) {
            it = playerWeapons.begin();
        } else if (it < playerWeapons.begin()) {
            it = playerWeapons.end() - 1;
        }
        currentPlayerWeapon = *it;
    }
}

void Level::advanceWaveIfNeeded()
{
    if (enemiesLeftToSpawn > 0 || !enemies.empty())
        return;
    wave++;
    enemiesLeftToSpawn = wave * 6;
    if (wave > 1)
    {
        currentEnemySpeed *= speedFactorIncreasePerWave;
        player->speedPxPerSecond *= speedFactorIncreasePerWave;
    }
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
            enemy->init(*this, *spawnPoint);
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
        Game::primitiveShapeHelper.drawRectOutline(collidedRectDst, {255, 0, 0, 255}, 2);
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
        Game::primitiveShapeHelper.drawRectOutline(checkAreaRectDst, {0, 255, 0, 255}, 1);
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
        Game::primitiveShapeHelper.drawRectOutline(checkAreaTileIndicesDst, {255, 255, 0, 255}, 2);
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
