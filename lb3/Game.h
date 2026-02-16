#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "PlayerHand.h"
#include "SaveLoadManager.h"
#include <string>
#include <memory>

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    EXIT
};

class Game {
private:
    GameField field;
    Player player;
    PlayerHand hand;
    
    GameState state;
    int currentLevel;
    bool running;
    std::string saveFileName;

    void processMovement(char input);
    void processSpell(char input);
    void showHelp() const;
    void checkGameState();
    void loadLevel(int level);
    void showMenu();
    void handleSaveLoad();

public:
    Game();
    void run();
};

#endif