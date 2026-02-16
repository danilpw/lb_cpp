#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    int health;
    int damage;
    int x;
    int y;

public:
    Player(); 
    Player(int health, int damage);

    void setPosition(int newX, int newY);

    int getX() const;
    int getY() const;

    int getHealth() const;
    int getDamage() const;

    void takeDamage(int value);
    bool isAlive() const;
};

#endif
