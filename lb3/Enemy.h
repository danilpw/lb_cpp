#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
private:
    int health;
    int damage;
    int x;
    int y;

public:
    Enemy();
    Enemy(int health, int damage);

    void setPosition(int newX, int newY);

    int getX() const;
    int getY() const;

    int getDamage() const;
    bool isAlive() const;

    void takeDamage(int value); // новый метод
};

#endif
