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

class Level 
{
public:
    Level();

    void load();
    void handleEvents();
    void update();
    void render();
    void dispose();

    std::map<int, Tile> tileset{};

    TileLayer tileLayer;
    TexturePool texturePool;
    Player* player = nullptr;
    Camera camera;
    Weapon* playerWeapon = nullptr;
    std::vector<Bullet*> bullets{};

    int horizontalTilesCount = 0;
    int verticalTilesCount = 0;
    int tileWidthPx = 0;
    int tileHeightPx = 0;
    int widthPx = 0;
    int heightPx = 0;

    SDL_Rect& buildTileRect(int x, int y) const;

    void assignWeaponToPlayer(int weaponId);
};

#endif // __SRC_LEVEL_LEVEL_HPP_
