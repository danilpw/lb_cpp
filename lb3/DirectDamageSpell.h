#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;

public:
    DirectDamageSpell(const std::string& name, int manaCost, int range, int damage);

    bool cast(GameField& field, Player& player, int targetX, int targetY) override;
};

#endif