#ifndef __SRC_GAMEENTITY_GAMEENTITY_HPP__
#define __SRC_GAMEENTITY_GAMEENTITY_HPP__

#include "../Core/Vector2D.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "../Core/TexturePool.hpp"
#include "../Level/Camera.hpp"
#include "../Core/Constants.hpp"
#include "../Core/Config.hpp"

class GameEntity
{
public:
    GameEntity();
    virtual ~GameEntity();

    std::string texturePath;
    Vector2D velocity;

    int width = 0;
    int height = 0;
    double rotation = 0;

    int damageAmount = -1;
    int maxHealth = -1;
    int currentHealth = -1;

    SDL_Rect collisionRect;
    SDL_Rect bulletCollisionRect;

    Vector2D position;
    Vector2D center;
    Vector2D positionPlusCenter;
    SDL_Texture* texture = nullptr;
    SDL_Rect dstRect;

    virtual void load(TexturePool& texturePool);
    virtual void update();
    virtual void draw(Camera& camera);

    void setPosition(Vector2D position);

    void setSize(int width, int height) { 
        this->width = width; 
        this->height = height; 
        this->center = Vector2D(width / 2, height / 2);
        this->collisionRect = {
            0,
            0,
            width,
            height
        };
    }

    #ifdef DEBUG_DRAW_COLLISION_RECTS
    void drawCollisionRect(Camera& camera);
    #endif

    bool pendingRemoval = false;
    void sendDamage(GameEntity* targetEntity);
    virtual void receiveDamage(const int amount);
};


#endif // __SRC_GAMEENTITY_GAMEENTITY_HPP__
