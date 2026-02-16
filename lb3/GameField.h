#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class GameField {
private:
    int width;
    int height;
    std::vector<Cell> cells;
    std::vector<Enemy> enemies;
    Player* player;

    int getIndex(int x, int y) const;

public:
    GameField(int w, int h);

    GameField(const GameField& other);
    GameField& operator=(const GameField& other);

    GameField(GameField&& other) noexcept;
    GameField& operator=(GameField&& other) noexcept;

    void placePlayer(Player& p, int x, int y);
    void placeEnemy(const Enemy& e, int x, int y);

    void movePlayer(Direction dir);
    void enemyTurn();

    void draw() const;
    
    bool allEnemiesDead() const { return enemies.empty(); }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::vector<Enemy>& getEnemies() { return enemies; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    Player* getPlayer() { return player; }
    const Player* getPlayer() const { return player; }
    Cell& getCell(int x, int y) { return cells[getIndex(x, y)]; }
    const Cell& getCell(int x, int y) const { return cells[getIndex(x, y)]; }
    
    void removeDeadEnemies();
};

#endif