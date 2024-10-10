#ifndef __SRC_LEVEL_LEVEL_HPP_
#define __SRC_LEVEL_LEVEL_HPP_

#include <map>
#include "Tile.hpp"
#include "TileLayer.hpp"
#include "../Core/TexturePool.hpp"
#include "../GameEntity/Player.hpp"
#include "Camera.hpp"
#include "../GameEntity/Weapon.hpp"
#include "../GameEntity/Bullet.hpp"
#include "../Core/Debouncer.hpp"

class Enemy;

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

    std::map<int, Tile> tileset{};

    TileLayer tileLayer;
    TexturePool texturePool;
    Player* player = nullptr;
    Camera camera;
    std::vector<Weapon*> playerWeapons{};
    Weapon* currentPlayerWeapon = nullptr;
    std::vector<Bullet*> bullets{};
    std::vector<Enemy*> enemies{};
    

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
    void cycleWeapon(int offset);

    #ifdef DEBUG_DRAW_COLLISION_RECTS
    std::vector<SDL_Rect> collidedRects{};
    std::vector<SDL_Rect> checkAreaRects{};
    std::vector<SDL_Rect> checkAreaTileIndicesRects{};
    void renderDebugShapes(Camera& camera);
    #endif
};

#endif // __SRC_LEVEL_LEVEL_HPP_
