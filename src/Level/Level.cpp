#include "Level.hpp"

#include "../Core/Macros.hpp"
#include "../Core/CollisionManager.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../Core/VectorUtils.hpp"
#include "../Screen/GameOver/GameOverScreen.hpp"
#include "../ScreenManager/ScreenManager.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../GameEntity/Player.hpp"
#include "../GameEntity/Weapon.hpp"
#include "../GameEntity/Bullet.hpp"
#include "../GameEntity/Enemy.hpp"
#include "../GameEntity/BloodPool.hpp"
#include "../GameEntity/Pickup.hpp"

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

    for (auto& bloodPool : bloodPools)
    {
        delete bloodPool;
    }
    bloodPools.clear();

    for (auto& pickup : pickups)
    {
        delete pickup;
    }
    pickups.clear();
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
    if (Game::control.isActionDown(CA_GAME_MOVE_UP) || Game::control.isActionDown(CA_GAME_MOVE_DOWN) || 
        Game::control.isActionDown(CA_GAME_MOVE_LEFT) || Game::control.isActionDown(CA_GAME_MOVE_RIGHT))
    {
        float speed = player->speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
        if (Game::control.isActionDown(CA_GAME_SPRINT) && player->stamina > 0.0f)
        {
            speed += player->sprintBoost * Game::latestLoopDeltaTimeSeconds;
            player->stamina -= player->staminaDecreaseRatePerSecond * Game::latestLoopDeltaTimeSeconds;
            if (player->stamina < 0.0f)
                player->stamina = 0.0f;
            player->lastStaminaDecreaseTime = std::chrono::steady_clock::now();
        }
        Vector2D velocity;
        velocity.y = Game::control.isActionDown(CA_GAME_MOVE_UP) ? -speed :
                              Game::control.isActionDown(CA_GAME_MOVE_DOWN) ? speed : 0;

        velocity.x = Game::control.isActionDown(CA_GAME_MOVE_LEFT) ? -speed :
                              Game::control.isActionDown(CA_GAME_MOVE_RIGHT) ? speed : 0;

        CollisionManager::processMovement(*player, velocity, *this, nullptr, player->isJumping());
        player->velocity = velocity;
    }
    else
    {
        player->velocity.reset();
    }

    // Jump
    if (Game::control.isActionDown(CA_GAME_JUMP))
    {
        player->onJumpRequest();
        Game::control.releaseAndBlockAction(CA_GAME_JUMP);
    }

    // Fire
    if (Game::control.isActionDown(CA_GAME_FIRE))
    {
        currentPlayerWeapon->onFireRequest([this](const Vector2D& position, float rotation)
        {
            Bullet* bullet = new Bullet(position, rotation, texturePool);
            bullets.push_back(bullet);
        });
    }

    // Reload
    if (Game::control.isActionDown(CA_GAME_RELOAD))
    {
        currentPlayerWeapon->reloadIfPossible();
    }

    // Cycle weapon
    if (Game::control.isActionDown(CA_GAME_PREVIOUS_WEAPON))
    {
        cycleWeapon(-1);
        Game::control.releaseAction(CA_GAME_PREVIOUS_WEAPON);
    }
    else if (Game::control.isActionDown(CA_GAME_NEXT_WEAPON))
    {
        cycleWeapon(1);
        Game::control.releaseAction(CA_GAME_NEXT_WEAPON);
    }

    // Weapon manual selection
    if (Game::control.isActionDown(CA_GAME_WEAPON_ID_1))
    {
        selectWeaponId(WeaponId::WEAPON_PISTOL);
        Game::control.releaseAndBlockAction(CA_GAME_WEAPON_ID_1);
    }
    if (Game::control.isActionDown(CA_GAME_WEAPON_ID_2))
    {
        selectWeaponId(WeaponId::WEAPON_SHOTGUN);
        Game::control.releaseAndBlockAction(CA_GAME_WEAPON_ID_2);
    }
    if (Game::control.isActionDown(CA_GAME_WEAPON_ID_3))
    {
        selectWeaponId(WeaponId::WEAPON_SMG);
        Game::control.releaseAndBlockAction(CA_GAME_WEAPON_ID_3);
    }
}

void Level::update()
{
    player->update(*this);
    camera.update();
    currentPlayerWeapon->update(*this);
    for (auto& bullet : bullets)
    {
        bullet->update(*this);
    }
    for (auto& enemy : enemies)
    {
        enemy->update(*this);
    }
    for (auto& pickup : pickups)
    {
        pickup->update(*this);
    }
    advanceWaveIfNeeded();
    spawnEnemiesIfNeeded();
    handleGameEntityPendingRemovals();
    bloodParticleManager.update();
}

void Level::render()
{
    tileLayer.render(camera, TileLayer::RENDER_FLAG_NON_COLLIDABLE_TILES);

    for (auto& bloodPool : bloodPools)
    {
        bloodPool->draw(camera);
    }

    for (auto& pickup : pickups)
    {
        pickup->draw(camera);
    }

    for (auto& bullet : bullets)
    {
        bullet->draw(camera);
    }
    
    for (auto& enemy : enemies)
    {
        enemy->draw(camera);
    }

    bloodParticleManager.render(camera.position);
    
    player->draw(camera);

    tileLayer.render(camera, TileLayer::RENDER_FLAG_COLLIDABLE_TILES);
    
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

void Level::onWeaponOrMagReceived(int weaponId)
{
    auto found = std::find_if(playerWeapons.begin(), playerWeapons.end(), [weaponId](const Weapon* weapon) {
        return weapon->id == weaponId;
    });

    Weapon* targetWeapon = nullptr;
    bool isNewWeapon = false;
    if (found == playerWeapons.end()) // Weapon not found in the player's inventory
    {
        targetWeapon = Weapon::createWeapon(weaponId, texturePool);
        playerWeapons.push_back(targetWeapon);
        targetWeapon->setOwner(*player);
        isNewWeapon = true;
    }
    else
    {
        targetWeapon = *found;
    }
    if (currentPlayerWeapon == nullptr || 
        (currentPlayerWeapon->id != weaponId && (currentPlayerWeapon->isOutOfAmmo() || isNewWeapon)))
    {
        currentPlayerWeapon = targetWeapon;
    }

    if (!targetWeapon->hasInfiniteMags)
        targetWeapon->availableMags += wave;
}

void Level::selectWeaponId(int weaponId)
{
    if (weaponId < 0 || weaponId >= WeaponId::WEAPON_ENUM_COUNT)
        return;
    auto found = std::find_if(playerWeapons.begin(), playerWeapons.end(), [weaponId](const Weapon* weapon) {
        return weapon->id == weaponId;
    });
    if (found == playerWeapons.end())
        return;
    currentPlayerWeapon = *found;
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
            attemptToSpawnPickup(enemy->position);
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

    VectorUtils::removeFromVectorIf(pickups, [](Pickup* pickup) {
        if (pickup->pendingRemoval)
        {
            delete pickup;
            return true;
        }
        return false;
    });

    if (player->pendingRemoval)
    {
        Game::screenManager.setScreen(new GameOverScreen(score, wave, id));
    }
}

void Level::createBloodPool(const Vector2D& position)
{
    BloodPool* bloodPool = new BloodPool(position);
    bloodPools.push_back(bloodPool);
}

void Level::attemptToSpawnPickup(const Vector2D position)
{
    for (int i = 0; i < PickupId::PICKUP_ENUM_COUNT; i++)
    {
        if (rand() % 100 < 15) // 15% chance to spawn a pickup for each iteration
        {
            Pickup* pickup = new Pickup(static_cast<PickupId>(i), position, texturePool);
            pickups.push_back(pickup);
            return;
        }
    }
}

void Level::collectPickup(Pickup* pickup)
{
    switch (pickup->id)
    {
    case PickupId::PICKUP_HEALTH:
        player->increaseHealth(1);
        break;
    case PickupId::PICKUP_SHOTGUN:
        onWeaponOrMagReceived(WeaponId::WEAPON_SHOTGUN);
        break;
    case PickupId::PICKUP_SMG:
        onWeaponOrMagReceived(WeaponId::WEAPON_SMG);
        break;
    
    default:
        break;
    }
    pickup->pendingRemoval = true;
}