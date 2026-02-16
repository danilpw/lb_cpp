#include "Game.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "SpellFactory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <limits>

Game::Game() 
    : field(10, 10), player(20, 3), hand(5, 3), 
      state(GameState::MENU), currentLevel(1), running(true),
      saveFileName("savegame.dat") {
    
    std::srand(std::time(nullptr));
}

void Game::showMenu() {
    std::cout << "\n=== MAIN MENU ===\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    switch(choice) {
        case 1:
            loadLevel(1);
            state = GameState::PLAYING;
            break;
        case 2:
            try {
                GameData data = SaveLoadManager::loadGame(saveFileName);
                
                currentLevel = data.currentLevel;
                player = data.player;
                
                field = GameField(10, 10);
                field.placePlayer(player, player.getX(), player.getY());
                
                for (const auto& enemy : data.enemies) {
                    field.placeEnemy(enemy, enemy.getX(), enemy.getY());
                }
                
                hand = PlayerHand(5, 3);
                hand.addSpell(SpellFactory::createFireball());
                if (data.spellCount > 1) {
                    hand.addSpell(SpellFactory::createExplosion());
                }
                
                std::cout << "Game loaded successfully!\n";
                state = GameState::PLAYING;
                
            } catch (const SaveLoadManager::LoadException& e) {
                std::cout << "Load failed: " << e.what() << "\n";
                std::cout << "Starting new game...\n";
                loadLevel(1);
                state = GameState::PLAYING;
            }
            break;
        case 3:
            state = GameState::EXIT;
            running = false;
            break;
        default:
            std::cout << "Invalid choice!\n";
    }
}

void Game::loadLevel(int level) {
    field = GameField(10, 10);
    field.placePlayer(player, 0, 0);
    
    hand = PlayerHand(5, 3);
    hand.addSpell(SpellFactory::createFireball());
    hand.addSpell(SpellFactory::createExplosion());
    
    field.getEnemies().clear();
    
    if (level == 1) {
        field.placeEnemy(Enemy(6, 2), 5, 5);
        field.placeEnemy(Enemy(6, 2), 8, 8);
        field.placeEnemy(Enemy(4, 3), 3, 7);
    } else if (level == 2) {
        field.placeEnemy(Enemy(8, 3), 4, 4);
        field.placeEnemy(Enemy(8, 3), 9, 9);
        field.placeEnemy(Enemy(6, 2), 2, 8);
        field.placeEnemy(Enemy(6, 2), 7, 3);
        field.placeEnemy(Enemy(5, 4), 10, 5);
    }
    
    currentLevel = level;
    std::cout << "Level " << level << " loaded!\n";
}

void Game::processMovement(char input) {
    Direction dir;
    if (input == 'w') dir = Direction::UP;
    else if (input == 's') dir = Direction::DOWN;
    else if (input == 'a') dir = Direction::LEFT;
    else if (input == 'd') dir = Direction::RIGHT;
    else return;
    
    field.movePlayer(dir);
}

void Game::processSpell(char input) {
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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    hand.useSpell(spellIndex, field, player, targetX, targetY);
    field.removeDeadEnemies();
}

void Game::checkGameState() {
    if (field.allEnemiesDead()) {
        field.draw();
        std::cout << "\n[MOVE:WASD] [SPELL:1-5] [SAVE:K] [LOAD:L] [EXIT:Q]\n";
        std::cout << "Level " << currentLevel << " complete!\n";
        
        if (currentLevel < 2) {
            std::cout << "Loading next level...\n";
            loadLevel(currentLevel + 1);
        } else {
            std::cout << "You win the game!\n";
            state = GameState::GAME_OVER;
            running = false;
        }
        return;
    }
    
    if (!player.isAlive()) {
        field.draw();
        std::cout << "\n[MOVE:WASD] [SPELL:1-5] [SAVE:K] [LOAD:L] [EXIT:Q]\n";
        std::cout << "Game Over\n";
        state = GameState::GAME_OVER;
        running = false;
    }
}

void Game::handleSaveLoad() {
    std::cout << "Enter filename (default: savegame.dat): ";
    std::string filename;
    std::cin >> filename;
    
    if (filename.empty()) {
        filename = saveFileName;
    }
    
    std::cout << "1. Save\n2. Load\nChoice: ";
    int choice;
    std::cin >> choice;
    
    try {
        if (choice == 1) {
            SaveLoadManager::saveGame(
                filename, 
                currentLevel, 
                player, 
                field.getEnemies(),
                hand.getSpellCount()
            );
            std::cout << "Game saved successfully!\n";
            
        } else if (choice == 2) {
            GameData data = SaveLoadManager::loadGame(filename);
            
            currentLevel = data.currentLevel;
            player = data.player;
            
            field = GameField(10, 10);
            field.placePlayer(player, player.getX(), player.getY());
            
            for (const auto& enemy : data.enemies) {
                field.placeEnemy(enemy, enemy.getX(), enemy.getY());
            }
            
            hand = PlayerHand(5, 3);
            hand.addSpell(SpellFactory::createFireball());
            if (data.spellCount > 1) {
                hand.addSpell(SpellFactory::createExplosion());
            }
            
            std::cout << "Game loaded successfully!\n";
        }
    } catch (const SaveLoadManager::SaveException& e) {
        std::cout << "Error saving: " << e.what() << "\n";
    } catch (const SaveLoadManager::LoadException& e) {
        std::cout << "Error loading: " << e.what() << "\n";
    }
}

void Game::run() {
    while (running) {
        if (state == GameState::MENU) {
            showMenu();
        } else if (state == GameState::PLAYING) {
            field.draw();
            
            std::cout << "\n[MOVE:WASD] [SPELL:1-5] [SAVE:K] [LOAD:L] [EXIT:Q]\n";
            
            hand.showSpells();
            
            std::cout << "\nYour move: ";
            char input;
            std::cin >> input;
            
            if (input == 'k') { 
                handleSaveLoad();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                continue;
            } else if (input == 'l') { 
                handleSaveLoad();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                continue;
            } else if (input == 'q') {
                std::cout << "\nReturn to menu? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    state = GameState::MENU;
                }
                continue;
            }
            
            if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
                processMovement(input);
            } else if (input >= '1' && input <= '5') {
                processSpell(input);
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
            
            field.enemyTurn();
            
            checkGameState();
        } else if (state == GameState::GAME_OVER) {
            std::cout << "\nPlay again? (y/n): ";
            char choice;
            std::cin >> choice;
            
            if (choice == 'y' || choice == 'Y') {
                state = GameState::MENU;
            } else {
                running = false;
            }
        }
    }
    
    std::cout << "Thanks for playing!\n";
}