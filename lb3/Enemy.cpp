#include "Enemy.h"

Enemy::Enemy() : health(0), damage(0), x(0), y(0) {} // Конструктор по умолчанию

Enemy::Enemy(int health, int damage)
    : health(health), damage(damage), x(0), y(0) {}

void Enemy::setPosition(int newX, int newY) { x = newX; y = newY; }

int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }
int Enemy::getDamage() const { return damage; }

bool Enemy::isAlive() const { return health > 0; }

void Enemy::takeDamage(int value) {
    health -= value;
    if (health < 0) health = 0;
}
