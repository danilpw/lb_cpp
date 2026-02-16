#include "GameField.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

GameField::GameField(int w, int h)
    : width(w), height(h), cells(w*h), player(nullptr) {
    std::srand(std::time(nullptr));
}

GameField::GameField(const GameField& other)
    : width(other.width),
      height(other.height),
      cells(other.cells),
      enemies(other.enemies),
      player(other.player) {}

GameField& GameField::operator=(const GameField& other) {
    if (this == &other) return *this;
    width = other.width;
    height = other.height;
    cells = other.cells;
    enemies = other.enemies;
    player = other.player;
    return *this;
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width),
      height(other.height),
      cells(std::move(other.cells)),
      enemies(std::move(other.enemies)),
      player(other.player) {
    other.player = nullptr;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this == &other) return *this;
    width = other.width;
    height = other.height;
    cells = std::move(other.cells);
    enemies = std::move(other.enemies);
    player = other.player;
    other.player = nullptr;
    return *this;
}

int GameField::getIndex(int x, int y) const { return y*width + x; }

void GameField::placePlayer(Player& p, int x, int y) {
    player = &p;
    player->setPosition(x, y);
    cells[getIndex(x,y)].placePlayer();
}

void GameField::placeEnemy(const Enemy& e, int x, int y) {
    Enemy enemy = e;
    enemy.setPosition(x, y);
    enemies.push_back(enemy);
    cells[getIndex(x,y)].placeEnemy();
}

void GameField::movePlayer(Direction dir) {
    if (!player) return;

    int x = player->getX();
    int y = player->getY();

    cells[getIndex(x,y)].removePlayer();

    if (dir == Direction::UP) y--;
    if (dir == Direction::DOWN) y++;
    if (dir == Direction::LEFT) x--;
    if (dir == Direction::RIGHT) x++;

    if (x<0 || x>=width || y<0 || y>=height) {
        cells[getIndex(player->getX(),player->getY())].placePlayer();
        return;
    }

    int index = getIndex(x,y);

    if (cells[index].containsEnemy()) {
        for (auto it = enemies.begin(); it != enemies.end(); ++it) {
            if (it->getX()==x && it->getY()==y) {
                it->takeDamage(player->getDamage());
                if (!it->isAlive()) {
                    cells[index].removeEnemy();
                    enemies.erase(it);
                }
                break;
            }
        }
        cells[getIndex(player->getX(),player->getY())].placePlayer();
        return;
    }

    player->setPosition(x,y);
    cells[index].placePlayer();
}

void GameField::enemyTurn() {
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        int ex = enemy.getX();
        int ey = enemy.getY();

        int nx = ex;
        int ny = ey;

        int dir = std::rand()%4;
        if (dir==0) nx--;
        else if (dir==1) nx++;
        else if (dir==2) ny--;
        else if (dir==3) ny++;

        if (nx<0 || nx>=width || ny<0 || ny>=height)
            continue;

        int idx = getIndex(nx,ny);

        if (cells[idx].containsPlayer()) {
            player->takeDamage(enemy.getDamage());
            continue;
        }

        if (!cells[idx].containsEnemy()) {
            cells[getIndex(ex,ey)].removeEnemy();
            enemy.setPosition(nx,ny);
            cells[idx].placeEnemy();
        }
    }
}

void GameField::draw() const {
    system("cls");
    for (int y=0;y<height;y++){
        for (int x=0;x<width;x++){
            const Cell& c = cells[getIndex(x,y)];
            if (c.containsPlayer()) std::cout<<"P ";
            else if (c.containsEnemy()) std::cout<<"E ";
            else std::cout<<". ";
        }
        std::cout<<"\n";
    }
    std::cout<<"HP: "<<player->getHealth()<<"\n";
}

void GameField::removeDeadEnemies() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!it->isAlive()) {
            int x = it->getX();
            int y = it->getY();
            cells[getIndex(x, y)].removeEnemy();
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}