#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "GameField.h"
#include "PlayerHand.h"
#include <memory>

class GameController {
private:
    GameField field;
    Player player;
    PlayerHand hand;
    bool running;

    void processMovement(char input);
    void processSpell(char input);
    void showHelp() const;
    void checkGameState();

public:
    GameController();
    void run();
};

#endif