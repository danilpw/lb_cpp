#include "AreaDamageSpell.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& name, int manaCost, int range, int damage, int areaSize)
    : Spell(name, manaCost, range), damage(damage), areaSize(areaSize) {}

bool AreaDamageSpell::cast(GameField& field, Player& player, int targetX, int targetY) {
    if (!isInRange(player.getX(), player.getY(), targetX, targetY)) {
        std::cout << "Target area is out of range!\n";
        return false;
    }

    if (targetX < 0 || targetX + areaSize > field.getWidth() ||
        targetY < 0 || targetY + areaSize > field.getHeight()) {
        std::cout << "Target area is outside the game field!\n";
        return false;
    }

    bool hitAnyEnemy = false;
    
    for (int dx = 0; dx < areaSize; dx++) {
        for (int dy = 0; dy < areaSize; dy++) {
            int currentX = targetX + dx;
            int currentY = targetY + dy;

            for (auto& enemy : field.getEnemies()) {
                if (enemy.getX() == currentX && enemy.getY() == currentY) {
                    enemy.takeDamage(damage);
                    hitAnyEnemy = true;
                    std::cout << "Enemy at (" << currentX << "," << currentY 
                              << ") takes " << damage << " damage!\n";
                }
            }
        }
    }

    if (!hitAnyEnemy) {
        std::cout << "Spell cast on empty area (" << targetX << "," << targetY << ")\n";
    }
    
    return true;
}