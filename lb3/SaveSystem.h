#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "GameField.h"
#include "Player.h"
#include "PlayerHand.h"

class SaveSystem {
public:
    class SaveException : public std::runtime_error {
    public:
        SaveException(const std::string& message) : std::runtime_error(message) {}
    };
    
    static void saveGame(const std::string& filename, 
                         const GameField& field, 
                         const Player& player,
                         const PlayerHand& hand);
    
    static void loadGame(const std::string& filename,
                         GameField& field,
                         Player& player,
                         PlayerHand& hand);
    
    static bool saveFileExists(const std::string& filename);
    

    static std::vector<std::string> getSaveFiles();
};

#endif