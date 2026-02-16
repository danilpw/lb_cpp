#ifndef SPELL_H
#define SPELL_H

#include <string>
#include "GameField.h"
#include "Enemy.h"

class Spell {
protected:
    std::string name;
    int manaCost;
    int range;

public:
    Spell(const std::string& name, int manaCost, int range);
    virtual ~Spell() = default;

    virtual bool cast(GameField& field, Player& player, int targetX, int targetY) = 0;
    
    std::string getName() const;
    int getManaCost() const;
    int getRange() const;
    bool isInRange(int playerX, int playerY, int targetX, int targetY) const;
};

#endif