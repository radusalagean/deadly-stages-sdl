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
#include "../GameEntity/Projectile.hpp"
#include "../GameEntity/Enemy.hpp"
#include "../GameEntity/BloodPool.hpp"
#include "../GameEntity/Pickup.hpp"
#include "../Core/AudioManager.hpp"

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

    for (auto& projectile : projectiles)
    {
        delete projectile;
    }
    projectiles.clear();

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
    loadAudio();
    for (auto& [id, tile] : tileset) 
    {
        tile.load();
    }

    player->load(texturePool);

    camera.mapBounds = new Vector2D(widthPx, heightPx);
    camera.target = &player->positionPlusCenter; // The camera will follow the player

    onWeaponOrMagReceived(WeaponId::WEAPON_PISTOL);

    #ifdef SUPPORTS_AIM_ASSIST
    auto texturePair = texturePool.loadIfNeededAndGet("res/image/crosshair.png", false);
    crosshairAimAssistTexture = texturePair.first;
    SDL_QueryTexture(crosshairAimAssistTexture, nullptr, nullptr, &crosshairAimAssistWidth, &crosshairAimAssistHeight);
    #endif
}

void Level::handleEvents()
{
    player->handleEvents(*this);

    const PressedActionData* pressedActionData = nullptr;

    // Jump
    if (Game::control.isActionDown(CA_GAME_JUMP, &pressedActionData))
    {
        player->onJumpRequest();
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }

    // Fire
    if (Game::control.isActionDown(CA_GAME_FIRE, &pressedActionData))
    {
        currentPlayerWeapon->onFireRequest(*pressedActionData, [this](const Vector2D& position, ProjectileType projectileType, int damage, float rotation)
        {
            Projectile* projectile = new Projectile(position, projectileType, damage, rotation, texturePool);
            projectiles.push_back(projectile);
        });
    }

    // Reload
    if (Game::control.isActionDown(CA_GAME_RELOAD, &pressedActionData))
    {
        currentPlayerWeapon->reloadIfPossible();
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }

    // Cycle weapon
    int cycleWeaponOffset = 0;
    if (Game::control.isActionDown(CA_GAME_PREVIOUS_WEAPON, &pressedActionData))
    {
        cycleWeaponOffset = -1;
    }
    else if (Game::control.isActionDown(CA_GAME_NEXT_WEAPON, &pressedActionData))
    {
        cycleWeaponOffset = 1;
    }
    if (cycleWeaponOffset != 0)
    {
        cycleWeapon(cycleWeaponOffset);
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }

    // Weapon manual selection
    if (Game::control.isActionDown(CA_GAME_WEAPON_SLOT_1, &pressedActionData))
    {
        selectWeaponId(WeaponId::WEAPON_PISTOL);
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
    if (Game::control.isActionDown(CA_GAME_WEAPON_SLOT_2, &pressedActionData))
    {
        selectWeaponId(WeaponId::WEAPON_SHOTGUN);
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
    if (Game::control.isActionDown(CA_GAME_WEAPON_SLOT_3, &pressedActionData))
    {
        selectWeaponId(WeaponId::WEAPON_SMG);
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }

    // Zoom
    if (Game::control.isActionDown(CA_GAME_ZOOM, &pressedActionData))
    {
        camera.toggleZoom();
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
}

void Level::update()
{
    player->update(*this);
    camera.update();
    currentPlayerWeapon->update(*this);
    for (auto& projectile : projectiles)
    {
        projectile->update(*this);
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
    #ifdef SUPPORTS_AIM_ASSIST
    updateCrosshairAimAssistIfNeeded();
    #endif
}

void Level::render()
{
    tileLayer.render(*this, TileLayer::RENDER_FLAG_NON_COLLIDABLE_TILES);

    for (auto& bloodPool : bloodPools)
    {
        bloodPool->draw(camera);
    }

    for (auto& pickup : pickups)
    {
        pickup->draw(camera);
    }

    for (auto& projectile : projectiles)
    {
        projectile->draw(camera);
    }
    
    for (auto& enemy : enemies)
    {
        enemy->draw(camera);
    }

    bloodParticleManager.render(camera);
    
    player->draw(camera);

    #ifdef SUPPORTS_AIM_ASSIST
    drawCrosshairAimAssistIfNeeded();
    #endif

    tileLayer.render(*this, TileLayer::RENDER_FLAG_COLLIDABLE_TILES);
    
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

    if (!targetWeapon->hasInfiniteAmmo)
        targetWeapon->ammoInInventory += targetWeapon->ammoPerReloadCycle * wave;
}

void Level::selectWeaponId(WeaponId weaponId)
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
        int x = camera.scale(collidedRect.x) - camera.position.x;
        int y = camera.scale(collidedRect.y) - camera.position.y;
        int w = camera.scale(collidedRect.w);
        int h = camera.scale(collidedRect.h);
        SDL_Rect collidedRectDst = { x, y, w, h };
        Game::primitiveShapeHelper.drawRectOutline(collidedRectDst, {255, 0, 0, 255}, 2);
    }
    collidedRects.clear();

    // Check area: Green
    for (const auto& checkAreaRect : checkAreaRects)
    {
        SDL_Rect checkAreaRectDst = 
        {
            static_cast<int>(camera.scale(checkAreaRect.x) - camera.position.x),
            static_cast<int>(camera.scale(checkAreaRect.y) - camera.position.y),
            static_cast<int>(camera.scale(checkAreaRect.w)),
            static_cast<int>(camera.scale(checkAreaRect.h))
        };
        Game::primitiveShapeHelper.drawRectOutline(checkAreaRectDst, {0, 255, 0, 255}, 1);
    }
    checkAreaRects.clear();

    // Tiles in the check area: Yellow
    for (const auto& checkAreaTileIndicesRect : checkAreaTileIndicesRects)
    {
        SDL_Rect checkAreaTileIndicesDst = 
        {
            static_cast<int>(camera.scale(checkAreaTileIndicesRect.x) - camera.position.x),
            static_cast<int>(camera.scale(checkAreaTileIndicesRect.y) - camera.position.y),
            static_cast<int>(camera.scale(checkAreaTileIndicesRect.w)),
            static_cast<int>(camera.scale(checkAreaTileIndicesRect.h))
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
            #ifdef SUPPORTS_AIM_ASSIST
            if (player != nullptr && player->targetEnemy == enemy)
            {
                player->onTargetEnemyRemoved(*this);
            }
            #endif
            if (enemy->bounty > 0)
                score += enemy->bounty;
            attemptToSpawnPickup(enemy->position);
            delete enemy;
            return true;
        }
        return false;
    });

    VectorUtils::removeFromVectorIf(projectiles, [](Projectile* projectile) {
        if (projectile->pendingRemoval)
        {
            delete projectile;
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
    BloodPool* bloodPool = new BloodPool(position, bloodPoolManager.getRandomBloodPoolTexture());
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
    Game::audioManager.playSound(AudioSFXId::PICKUP);
}

#ifdef SUPPORTS_AIM_ASSIST
void Level::updateCrosshairAimAssistIfNeeded()
{
    if (player->targetEnemy == nullptr)
        return;
    Vector2D targetEnemyPositionCenter = player->targetEnemy->positionPlusCenter;
    crosshairAimAssistDstRect.x = camera.scale(targetEnemyPositionCenter.x - crosshairAimAssistWidth / 2) - camera.position.x;
    crosshairAimAssistDstRect.y = camera.scale(targetEnemyPositionCenter.y - crosshairAimAssistHeight / 2) - camera.position.y;
    crosshairAimAssistDstRect.w = camera.scale(crosshairAimAssistWidth);
    crosshairAimAssistDstRect.h = camera.scale(crosshairAimAssistHeight);
}

void Level::drawCrosshairAimAssistIfNeeded()
{
    if (player->targetEnemy == nullptr)
        return;
    SDL_RenderCopy(Game::renderer, crosshairAimAssistTexture, nullptr, &crosshairAimAssistDstRect);
}
#endif

void Level::loadAudio() 
{
    std::vector<AudioSFXId> levelSounds = 
    {
        // Pistol
        AudioSFXId::PISTOL_FIRE,
        AudioSFXId::PISTOL_DRY_FIRE,
        AudioSFXId::PISTOL_RELOAD,
        // Shotgun
        AudioSFXId::SHOTGUN_FIRE,
        AudioSFXId::SHOTGUN_DRY_FIRE,
        AudioSFXId::SHOTGUN_PUMP,
        AudioSFXId::SHOTGUN_RELOAD,
        // SMG
        AudioSFXId::SMG_FIRE,
        AudioSFXId::SMG_DRY_FIRE,
        AudioSFXId::SMG_RELOAD,
        // Mobs
        AudioSFXId::JUMP,
        AudioSFXId::LAND_FROM_JUMP,
        AudioSFXId::CRUSH,
        AudioSFXId::PLAYER_HURT,
        // Pickup
        AudioSFXId::PICKUP
    };

    std::vector<AudioMusicId> levelMusic = 
    {
        
    };

    Game::audioManager.loadSoundList(levelSounds);
    Game::audioManager.loadMusicList(levelMusic);
}
