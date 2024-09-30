#ifndef __SRC_GAMEENTITY_GAMEENTITY_HPP__
#define __SRC_GAMEENTITY_GAMEENTITY_HPP__

#include "../Core/Vector2D.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "../Core/TexturePool.hpp"
#include "../Level/Camera.hpp"
#include "../Core/Constants.hpp"

class GameEntity
{
public:
    GameEntity();
    virtual ~GameEntity();

    std::string texturePath;
    Vector2D velocity;

    int width;
    int height;
    double rotation = 0;

    SDL_Rect collisionRect;

    Vector2D position;
    SDL_Texture* texture = nullptr;
    SDL_Rect dstRect;

    virtual void load(TexturePool& texturePool);
    virtual void update();
    virtual void draw(Camera& camera);

    Vector2D* getPosition();
    void setPosition(Vector2D position);

    void setSize(int width, int height) { this->width = width; this->height = height; }
};


#endif // __SRC_GAMEENTITY_GAMEENTITY_HPP__
