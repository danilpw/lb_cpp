#include "Player.h"

Player::Player(int health, int damage)
    : health(health), damage(damage), x(0), y(0) {}

void Player::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

int Player::getX() const { return x; }
int Player::getY() const { return y; }

int Player::getHealth() const { return health; }
int Player::getDamage() const { return damage; }

void Player::takeDamage(int value) {
    health -= value;
}

bool Player::isAlive() const {
    return health > 0;
}
