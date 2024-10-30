#ifndef __SRC_LEVEL_LEVEL_HPP_
#define __SRC_LEVEL_LEVEL_HPP_

#include <map>
#include "Tile.hpp"
#include "../Core/TexturePool.hpp"
#include "Camera.hpp"
#include "../Core/Debouncer.hpp"
#include "../BloodParticle/BloodParticleManager.hpp"
#include "../Core/Config.hpp"
#include "TileLayer.hpp"
#include "../GameEntity/Weapon.hpp"
#include "../BloodPoolManager/BloodPoolManager.hpp"
#include "../Core/Macros.hpp"

class Player;
class Weapon;
class Projectile;
class Enemy;
class BloodPool;
class Pickup;

class Level 
{
public:
    Level(std::string id);
    ~Level();

    void load();
    void handleEvents();
    void update();
    void render();

    std::string id;
    std::string levelMusicPath;

    std::map<int, Tile> tileset{};

    TileLayer tileLayer;
    TexturePool texturePool;
    BloodPoolManager bloodPoolManager;
    Player* player = nullptr;
    Camera camera;
    std::vector<Weapon*> playerWeapons{};
    Weapon* currentPlayerWeapon = nullptr;
    std::vector<Projectile*> projectiles{};
    std::vector<Enemy*> enemies{};
    std::vector<BloodPool*> bloodPools;
    std::vector<Pickup*> pickups{};
    int horizontalTilesCount = 0;
    int verticalTilesCount = 0;
    int tileWidthPx = 0;
    int tileHeightPx = 0;
    int widthPx = 0;
    int heightPx = 0;

    int score = 0;
    int wave = 0;
    int enemiesLeftToSpawn = 0;
    int getEnemiesLeft();
    const float speedFactorIncreasePerWave = 1.05f;
    float currentEnemySpeed = 100.0f;
    std::vector<Vector2D*> spawnPoints{};
    Debouncer spawnEnemyDebouncer = Debouncer(1000);
    void advanceWaveIfNeeded();
    void spawnEnemiesIfNeeded();
    void handleGameEntityPendingRemovals();

    SDL_Rect buildTileRect(int x, int y) const;

    void onWeaponOrMagReceived(WeaponId weaponId);
    void selectWeaponId(WeaponId weaponId);
    void cycleWeapon(int offset);

    #ifdef DEBUG_DRAW_COLLISION_RECTS
    std::vector<SDL_Rect> collidedRects{};
    std::vector<SDL_Rect> checkAreaRects{};
    std::vector<SDL_Rect> checkAreaTileIndicesRects{};
    void renderDebugShapes(Camera& camera);
    #endif

    BloodParticleManager bloodParticleManager;

    void createBloodPool(const Vector2D& position);

    void attemptToSpawnPickup(const Vector2D position);
    void collectPickup(Pickup* pickup);

    #ifdef SUPPORTS_AIM_ASSIST
    SDL_Texture* crosshairAimAssistTexture = nullptr;
    int crosshairAimAssistWidth = 0;
    int crosshairAimAssistHeight = 0;
    SDL_Rect crosshairAimAssistDstRect;
    void updateCrosshairAimAssistIfNeeded();
    void drawCrosshairAimAssistIfNeeded();
    #endif

    void loadAudio();
};

#endif // __SRC_LEVEL_LEVEL_HPP_
