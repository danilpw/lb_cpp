#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include <memory>
#include <ctime>  
#include <cstdlib> 
#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"

class SpellFactory {
public:
    static std::unique_ptr<Spell> createRandomSpell() {
        static bool seeded = false;
        if (!seeded) {
            std::srand(std::time(nullptr));
            seeded = true;
        }
        
        if (std::rand() % 2 == 0) {
            return std::make_unique<DirectDamageSpell>("Fireball", 10, 5, 3);
        } else {
            return std::make_unique<AreaDamageSpell>("Explosion", 15, 4, 2, 2);
        }
    }
    
    static std::unique_ptr<Spell> createFireball() {
        return std::make_unique<DirectDamageSpell>("Fireball", 10, 5, 3);
    }
    
    static std::unique_ptr<Spell> createExplosion() {
        return std::make_unique<AreaDamageSpell>("Explosion", 15, 4, 2, 2);
    }
};

#endif