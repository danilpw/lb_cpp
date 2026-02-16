#include "Cell.h"

Cell::Cell() : hasPlayer(false), hasEnemy(false) {}

void Cell::placePlayer() { hasPlayer = true; }
void Cell::removePlayer() { hasPlayer = false; }

void Cell::placeEnemy() { hasEnemy = true; }
void Cell::removeEnemy() { hasEnemy = false; }

bool Cell::containsPlayer() const { return hasPlayer; }
bool Cell::containsEnemy() const { return hasEnemy; }
