#ifndef __SRC_GAMEENTITY_ENEMY_HPP__
#define __SRC_GAMEENTITY_ENEMY_HPP__

#include "GameEntity.hpp"
#include "../Level/Level.hpp"
#include "../Core/Config.hpp"

class Enemy : public GameEntity
{
public:
    Enemy();
    ~Enemy();

    void init(Level& level, Vector2D& spawnPoint);

    // Speed
    float topSpeedPxPerSecond = 0.0f;
    float speedPxPerSecond = 0.0f;

    // Damage
    void receiveDamage(const int amount, Level& level);
    void crush(Level& level);

    // Contact with player
    const int hitAfterContactDelayMs = 300;
    bool isInContactWithPlayer = false;
    std::chrono::steady_clock::time_point contactWithPlayerStartTime;
    void handleContactWithPlayer(Player* player, Level& level);

    Vector2D* target = nullptr;

    void update(Level& level);
    void draw(Camera& camera);

};

#endif // __SRC_GAMEENTITY_ENEMY_HPP__
