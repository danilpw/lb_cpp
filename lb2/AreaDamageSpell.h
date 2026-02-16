#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int areaSize;

public:
    AreaDamageSpell(const std::string& name, int manaCost, int range, int damage, int areaSize = 2);

    bool cast(GameField& field, Player& player, int targetX, int targetY) override;
};

#endif