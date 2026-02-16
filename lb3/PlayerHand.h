#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include <vector>
#include <memory>
#include "Spell.h"
#include "Player.h"

class PlayerHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;
    int killsForNewSpell; 
    int currentKills;

public:
    PlayerHand(int maxSize, int killsForNewSpell = 3);

    void addSpell(std::unique_ptr<Spell> spell);
    bool useSpell(int spellIndex, GameField& field, Player& player, int targetX, int targetY);
    void onEnemyKilled();
    
    void showSpells() const;
    bool isEmpty() const;
    int getSpellCount() const;
    std::string getSpellName(int index) const;
};

#endif