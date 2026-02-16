#include "GameController.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

GameController::GameController() 
    : field(10, 10), player(20, 3), hand(5, 3), running(true) {
    
    std::srand(std::time(nullptr));
    
    field.placePlayer(player, 0, 0);
    
    if (std::rand() % 2 == 0) {
        hand.addSpell(std::make_unique<DirectDamageSpell>("Fireball", 10, 5, 3));
    } else {
        hand.addSpell(std::make_unique<AreaDamageSpell>("Explosion", 15, 4, 2, 2));
    }

    field.placeEnemy(Enemy(6, 2), 3, 5);
    field.placeEnemy(Enemy(6, 2), 6, 7);
    field.placeEnemy(Enemy(4, 3), 2, 3);
    field.placeEnemy(Enemy(5, 2), 8, 8);
}

void GameController::processMovement(char input) {
    Direction dir;
    if (input == 'w') dir = Direction::UP;
    else if (input == 's') dir = Direction::DOWN;
    else if (input == 'a') dir = Direction::LEFT;
    else if (input == 'd') dir = Direction::RIGHT;
    else return;
    
    field.movePlayer(dir);
}

void GameController::processSpell(char input) {
    int spellIndex = input - '1';
    
    if (spellIndex >= hand.getSpellCount()) {
        std::cout << "No spell at this position!\n";
        return;
    }
    
    std::cout << "Enter target coordinates (x y): ";
    int targetX, targetY;
    std::cin >> targetX >> targetY;
    
    if (std::cin.fail()) {
        std::cout << "Invalid coordinates!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }
    
    hand.useSpell(spellIndex, field, player, targetX, targetY);
}

void GameController::showHelp() const {
    std::cout << "\n=== Controls ===\n";
    std::cout << "W/A/S/D - move\n";
    std::cout << "1-5 - use spell\n";
    std::cout << "H - show help\n";
    std::cout << "================\n\n";
}

void GameController::checkGameState() {
    if (field.allEnemiesDead()) {
        field.draw();
        std::cout << "You win!\n";
        running = false;
    }
    
    if (!player.isAlive()) {
        field.draw();
        std::cout << "Game Over\n";
        running = false;
    }
}

void GameController::run() {
    std::cout << "=== Game Started ===\n";
    showHelp();
    
    while (running) {
        field.draw();
        hand.showSpells();
        
        std::cout << "\nYour move: ";
        char input;
        std::cin >> input;
        
        if (input == 'h') {
            showHelp();
            continue;
        }
        
        if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
            processMovement(input);
        }
        else if (input >= '1' && input <= '5') {
            processSpell(input);
        }
        
        field.enemyTurn();
        checkGameState();
    }
}