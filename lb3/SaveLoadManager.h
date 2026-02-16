#ifndef SAVE_LOAD_MANAGER_H
#define SAVE_LOAD_MANAGER_H

#include "GameField.h"
#include "Player.h"
#include "PlayerHand.h"
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

struct GameData {
    int currentLevel;
    Player player;
    std::vector<Enemy> enemies;
    int spellCount;
};

class SaveLoadManager {
public:
    class SaveException : public std::exception {
        std::string message;
    public:
        SaveException(const std::string& msg) : message(msg) {}
        const char* what() const noexcept override { return message.c_str(); }
    };
    
    class LoadException : public std::exception {
        std::string message;
    public:
        LoadException(const std::string& msg) : message(msg) {}
        const char* what() const noexcept override { return message.c_str(); }
    };

    static void saveGame(const std::string& filename, int currentLevel, 
                         const Player& player, const std::vector<Enemy>& enemies,
                         int spellCount) {
        try {
            std::ofstream file(filename, std::ios::binary);
            if (!file.is_open()) {
                throw SaveException("Cannot open file for writing");
            }

            file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
            
            file.write(reinterpret_cast<const char*>(&player), sizeof(Player));
            
            size_t enemyCount = enemies.size();
            file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
            for (const auto& enemy : enemies) {
                file.write(reinterpret_cast<const char*>(&enemy), sizeof(Enemy));
            }
            
            file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
            
            file.close();
            
        } catch (const std::exception& e) {
            throw SaveException(std::string("Save failed: ") + e.what());
        }
    }

    static GameData loadGame(const std::string& filename) {
        try {
            std::ifstream file(filename, std::ios::binary);
            if (!file.is_open()) {
                throw LoadException("Save file does not exist");
            }

            if (file.peek() == std::ifstream::traits_type::eof()) {
                throw LoadException("Save file is empty");
            }

            GameData data;
            
            file.read(reinterpret_cast<char*>(&data.currentLevel), sizeof(data.currentLevel));
            
            file.read(reinterpret_cast<char*>(&data.player), sizeof(Player));
            
            if (file.fail()) {
                throw LoadException("Corrupted save file");
            }

            size_t enemyCount;
            file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
            
            if (enemyCount > 100) { // Разумный лимит
                throw LoadException("Invalid enemy count in save file");
            }

            data.enemies.clear();
            for (size_t i = 0; i < enemyCount; i++) {
                Enemy enemy(0, 0);
                file.read(reinterpret_cast<char*>(&enemy), sizeof(Enemy));
                if (file.fail()) {
                    throw LoadException("Failed to read enemy data");
                }
                data.enemies.push_back(enemy);
            }

            file.read(reinterpret_cast<char*>(&data.spellCount), sizeof(data.spellCount));
            
            file.close();
            
            return data;
            
        } catch (const std::exception& e) {
            throw LoadException(std::string("Load failed: ") + e.what());
        }
    }

    static bool saveFileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
};

#endif