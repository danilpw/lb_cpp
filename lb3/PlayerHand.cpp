#include "PlayerHand.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

PlayerHand::PlayerHand(int maxSize, int killsForNewSpell)
    : maxSize(maxSize), killsForNewSpell(killsForNewSpell), currentKills(0) {
    std::srand(std::time(nullptr));
}

void PlayerHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        std::cout << "New spell added to hand!\n";
    } else {
        std::cout << "Hand is full! Cannot add more spells.\n";
    }
}

bool PlayerHand::useSpell(int spellIndex, GameField& field, Player& player, int targetX, int targetY) {
    if (spellIndex < 0 || spellIndex >= spells.size()) {
        std::cout << "Invalid spell index!\n";
        return false;
    }

    bool success = spells[spellIndex]->cast(field, player, targetX, targetY);
    
    if (success) {
        spells.erase(spells.begin() + spellIndex);
        std::cout << "Spell used and removed from hand.\n";
    }
    
    return success;
}

void PlayerHand::onEnemyKilled() {
    currentKills++;
    std::cout << "Kills: " << currentKills << "/" << killsForNewSpell << "\n";
    
    if (currentKills >= killsForNewSpell) {
        currentKills = 0;
        std::cout << "You earned a new spell!\n";
    }
}

void PlayerHand::showSpells() const {
    if (spells.empty()) {
        std::cout << "Hand is empty\n";
        return;
    }
    
    std::cout << "Available spells:\n";
    for (size_t i = 0; i < spells.size(); i++) {
        std::cout << i + 1 << ". " << spells[i]->getName() 
                  << " (Mana: " << spells[i]->getManaCost() << ")\n";
    }
}

bool PlayerHand::isEmpty() const { return spells.empty(); }
int PlayerHand::getSpellCount() const { return spells.size(); }

std::string PlayerHand::getSpellName(int index) const {
    if (index >= 0 && index < spells.size()) {
        return spells[index]->getName();
    }
    return "";
}