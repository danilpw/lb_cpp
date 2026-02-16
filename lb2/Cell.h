#ifndef CELL_H
#define CELL_H

class Cell {
private:
    bool hasPlayer;
    bool hasEnemy;

public:
    Cell();

    void placePlayer();
    void removePlayer();

    void placeEnemy();
    void removeEnemy();

    bool containsPlayer() const;
    bool containsEnemy() const;
};

#endif
