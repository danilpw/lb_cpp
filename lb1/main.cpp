#include <iostream>
#include "GameField.h"

int main() {
    GameField field(10,10);
    Player player(6,2);

    field.placePlayer(player,0,0);

    field.placeEnemy(Enemy(6,2),3,5);
    field.placeEnemy(Enemy(6,2),6,7);

    while (player.isAlive()) {
        field.draw();

        if (field.allEnemiesDead()) {
            std::cout<<"You win!\n";
            break;
        }

        std::cout<<"Move (W/A/S/D): ";
        char input;
        std::cin>>input;

        if (input=='w') field.movePlayer(Direction::UP);
        if (input=='s') field.movePlayer(Direction::DOWN);
        if (input=='a') field.movePlayer(Direction::LEFT);
        if (input=='d') field.movePlayer(Direction::RIGHT);

        field.enemyTurn();

        if (!player.isAlive()) {
            field.draw();
            std::cout<<"Game Over\n";
            break;
        }
    }

    return 0;
}
