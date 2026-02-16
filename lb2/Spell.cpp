#include "Spell.h"
#include <cmath>

Spell::Spell(const std::string& name, int manaCost, int range)
    : name(name), manaCost(manaCost), range(range) {}

std::string Spell::getName() const { return name; }
int Spell::getManaCost() const { return manaCost; }
int Spell::getRange() const { return range; }

bool Spell::isInRange(int playerX, int playerY, int targetX, int targetY) const {
    int dx = std::abs(playerX - targetX);
    int dy = std::abs(playerY - targetY);
    return (dx <= range && dy <= range);
}