#include "DirectDamageSpell.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& name, int manaCost, int range, int damage)
    : Spell(name, manaCost, range), damage(damage) {}

bool DirectDamageSpell::cast(GameField& field, Player& player, int targetX, int targetY) {
    if (!isInRange(player.getX(), player.getY(), targetX, targetY)) {
        std::cout << "Target is out of range!\n";
        return false;
    }

    for (auto& enemy : field.getEnemies()) {
        if (enemy.getX() == targetX && enemy.getY() == targetY) {
            enemy.takeDamage(damage);
            std::cout << "Spell " << name << " deals " << damage 
                      << " damage to enemy at (" << targetX << "," << targetY << ")\n";
            return true;
        }
    }

    std::cout << "No enemy at target position!\n";
    return false;
}