#include "GameEntity.hpp"

#include "../Game.hpp"
#include "../Core/Macros.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../Core/CollisionManager.hpp"
#include "../Level/Level.hpp"
#include "../Core/SDLUtils.hpp"

GameEntity::GameEntity()
{

}

GameEntity::~GameEntity()
{

}

void GameEntity::load(TexturePool& texturePool)
{
    auto texturePair = texturePool.loadIfNeededAndGet(texturePath, castShadow);
    texture = texturePair.first;
    if (texturePair.second != nullptr)
        shadowTexture = texturePair.second;
}

void GameEntity::update(Level& level)
{
    position += velocity;
    positionPlusCenter = position + center;
    refreshCollisionData();
}

void GameEntity::draw(Camera& camera)
{
    if (dstRectScale == 1.0f)
    {
        dstRect.w = GSCALE(width);
        dstRect.h = GSCALE(height);
        dstRect.x = GSCALE(position.x) - camera.position.x;
        dstRect.y = GSCALE(position.y) - camera.position.y;
    }
    else
    {
        dstRect.w = GSCALE(width * dstRectScale);
        dstRect.h = GSCALE(height * dstRectScale);
        dstRect.x = GSCALE(positionPlusCenter.x - (dstRect.w / 2)) - camera.position.x;
        dstRect.y = GSCALE(positionPlusCenter.y - (dstRect.h / 2)) - camera.position.y;
    }

    if (castShadow && shadowTexture != nullptr)
    {
        float scale = 1.18f;
        if (dstRectScale > 1.0f)
            scale *= 1.0f + ((dstRectScale - 1.0f) * 0.4f);
        shadowDstRect.w = static_cast<int>(dstRect.w * scale);
        shadowDstRect.h = static_cast<int>(dstRect.h * scale);
        shadowDstRect.x = dstRect.x + (dstRect.w - shadowDstRect.w) / 2;
        shadowDstRect.y = dstRect.y + (dstRect.h - shadowDstRect.h) / 2;

        SDL_RenderCopyEx(Game::renderer, shadowTexture, NULL, &shadowDstRect, rotation, NULL, SDL_FLIP_NONE);
    }

    SDL_RenderCopyEx(Game::renderer, texture, NULL, &dstRect, rotation, NULL, SDL_FLIP_NONE);
}

void GameEntity::setPosition(Vector2D position)
{
    this->position = position;
    this->positionPlusCenter = position + center;
    this->refreshCollisionData();
}

void GameEntity::setSize(int width, int height)
{
    this->width = width; 
    this->height = height; 
    this->center = Vector2D(width / 2, height / 2);
    this->collisionRect = 
    {
        0,
        0,
        width,
        height
    };
    this->collisionLine = 
    {
        {width / 2.0f, 0.0f},
        {width / 2.0f, static_cast<float>(height)}
    };
    this->refreshCollisionData();
}

void GameEntity::setRotation(float angle)
{
    // Only update if the angle has actually changed
    if (this->rotation != angle) {
        this->rotation = angle;

        // Rotate the collision line based on the entity's rotation
        float radians = rotation * M_PI / 180.0f;
        float cosAngle = cos(radians);
        float sinAngle = sin(radians);

        // Reset collision line to its original position
        collisionLine.start = {width / 2.0f, 0.0f};
        collisionLine.end = {width / 2.0f, static_cast<float>(height)};

        // Rotate the start point of the collision line
        float startX = collisionLine.start.x - center.x;
        float startY = collisionLine.start.y - center.y;
        collisionLine.start.x = center.x + (startX * cosAngle) - (startY * sinAngle);
        collisionLine.start.y = center.y + (startX * sinAngle) + (startY * cosAngle);

        // Rotate the end point of the collision line
        float endX = collisionLine.end.x - center.x;
        float endY = collisionLine.end.y - center.y;
        collisionLine.end.x = center.x + (endX * cosAngle) - (endY * sinAngle);
        collisionLine.end.y = center.y + (endX * sinAngle) + (endY * cosAngle);
    }
}

void GameEntity::refreshCollisionData()
{
    positionPlusCollisionRect.x = position.x + collisionRect.x;
    positionPlusCollisionRect.y = position.y + collisionRect.y;
    positionPlusCollisionRect.w = collisionRect.w;
    positionPlusCollisionRect.h = collisionRect.h;

    positionPlusCollisionLine.start.x = position.x + collisionLine.start.x;
    positionPlusCollisionLine.start.y = position.y + collisionLine.start.y;
    positionPlusCollisionLine.end.x = position.x + collisionLine.end.x;
    positionPlusCollisionLine.end.y = position.y + collisionLine.end.y;
}

#ifdef DEBUG_DRAW_COLLISION_RECTS
void GameEntity::drawCollisionRect(Camera& camera) 
{
    // Draw collision rect with blue
    SDL_Rect debugRect; 
    debugRect.x = GSCALE(positionPlusCollisionRect.x) - camera.position.x;
    debugRect.y = GSCALE(positionPlusCollisionRect.y) - camera.position.y;
    debugRect.w = GSCALE(collisionRect.w);
    debugRect.h = GSCALE(collisionRect.h);
    Game::primitiveShapeHelper.drawRectOutline(debugRect, {0, 0, 255, 255}, 2);

    // Draw collision line with red
    Line debugLine;
    debugLine.start.x = GSCALE(positionPlusCollisionLine.start.x) - camera.position.x;
    debugLine.start.y = GSCALE(positionPlusCollisionLine.start.y) - camera.position.y;
    debugLine.end.x = GSCALE(positionPlusCollisionLine.end.x) - camera.position.x;
    debugLine.end.y = GSCALE(positionPlusCollisionLine.end.y) - camera.position.y;
    Game::primitiveShapeHelper.drawLine(debugLine, {255, 0, 0, 255});
}
#endif

void GameEntity::sendDamage(GameEntity* targetEntity)
{
    targetEntity->receiveDamage(damageAmount);
}

void GameEntity::receiveDamage(const int amount)
{
    if (currentHealth <= 0)
        return;
    currentHealth = std::max(0, currentHealth - amount);
    if (currentHealth == 0)
        pendingRemoval = true;
}
