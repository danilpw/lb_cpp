#include "SaveSystem.h"
#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <unistd.h>
#include <sys/types.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

void SaveSystem::saveGame(const std::string& filename, 
                          const GameField& field, 
                          const Player& player,
                          const PlayerHand& hand) {
    try {
        MKDIR("saves");
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw SaveException("Cannot open file for writing: " + filename);
        }
        
        file << "SAVEGAME_V1\n";
        
        file << "PLAYER " << player.getHealth() << " " << player.getDamage() 
             << " " << player.getX() << " " << player.getY() << "\n";
        
        file << "FIELD " << field.getWidth() << " " << field.getHeight() << "\n";
        
        const auto& enemies = field.getEnemies();
        file << "ENEMIES " << enemies.size() << "\n";
        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                file << "ENEMY " << " " << enemy.getDamage() 
                     << " " << enemy.getX() << " " << enemy.getY() << "\n";
            }
        }
        
        file.close();
        std::cout << "Game saved successfully to " << filename << "\n";
        
    } catch (const SaveException&) {
        throw;
    } catch (const std::exception& e) {
        throw SaveException(std::string("Error saving game: ") + e.what());
    }
}

void SaveSystem::loadGame(const std::string& filename,
                         GameField& field,
                         Player& player,
                         PlayerHand& hand) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw SaveException("Cannot open file for reading: " + filename);
        }
        
        std::string line;
        if (!std::getline(file, line) || line != "SAVEGAME_V1") {
            throw SaveException("Invalid or corrupted save file format");
        }
        
        int expectedEnemies = 0;
        std::vector<Enemy> loadedEnemies;
        int playerHealth = 0, playerDamage = 0, playerX = 0, playerY = 0;
        int fieldWidth = 0, fieldHeight = 0;
        bool playerLoaded = false;
        bool fieldLoaded = false;
        
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            
            if (type == "PLAYER") {
                if (!(iss >> playerHealth >> playerDamage >> playerX >> playerY)) {
                    throw SaveException("Invalid PLAYER data in save file");
                }
                playerLoaded = true;
            }
            else if (type == "FIELD") {
                if (!(iss >> fieldWidth >> fieldHeight)) {
                    throw SaveException("Invalid FIELD data in save file");
                }
                fieldLoaded = true;
            }
            else if (type == "ENEMIES") {
                if (!(iss >> expectedEnemies)) {
                    throw SaveException("Invalid ENEMIES count in save file");
                }
            }
            else if (type == "ENEMY") {
                int health, damage, x, y;
                if (!(iss >> health >> damage >> x >> y)) {
                    throw SaveException("Invalid ENEMY data in save file");
                }
                
                if (!fieldLoaded) {
                    throw SaveException("ENEMY data before FIELD data");
                }
                
                // Проверяем корректность координат
                if (x < 0 || x >= fieldWidth || y < 0 || y >= fieldHeight) {
                    throw SaveException("Enemy position out of bounds");
                }
                
                Enemy enemy(health, damage);
                enemy.setPosition(x, y);
                loadedEnemies.push_back(enemy);
            }
        }
        
        file.close();
        
        if (!playerLoaded) {
            throw SaveException("No PLAYER data in save file");
        }
        if (!fieldLoaded) {
            throw SaveException("No FIELD data in save file");
        }
        if (loadedEnemies.size() != expectedEnemies) {
            throw SaveException("Enemy count mismatch in save file");
        }

        player = Player(playerHealth, playerDamage);
        player.setPosition(playerX, playerY);

        GameField newField(fieldWidth, fieldHeight);
        field = std::move(newField);
        
        field.placePlayer(player, playerX, playerY);

        for (const auto& enemy : loadedEnemies) {
            field.placeEnemy(enemy, enemy.getX(), enemy.getY());
        }
        
        std::cout << "Game loaded successfully from " << filename << "\n";
        
    } catch (const SaveException&) {
        throw;
    } catch (const std::exception& e) {
        throw SaveException(std::string("Error loading game: ") + e.what());
    }
}

bool SaveSystem::saveFileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

std::vector<std::string> SaveSystem::getSaveFiles() {
    std::vector<std::string> saveFiles;
    
    MKDIR("saves");
    
    DIR* dir = opendir("saves");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            if (filename.length() > 4 && 
                filename.substr(filename.length() - 4) == ".sav") {
                saveFiles.push_back(filename);
            }
        }
        closedir(dir);
    }
    
    return saveFiles;
}