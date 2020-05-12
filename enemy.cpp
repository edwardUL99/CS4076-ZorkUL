#include "enemy.h"

                                                                               // 9. Initializer list
Enemy::Enemy(string name, int staminaAttackCost, int damage, Potion *potion) : Character(name) {
    this->staminaAttackCost = staminaAttackCost;
    this->damage = damage;
    this->potion = potion;
}

int Enemy::getStaminaAttackCost() {
    return staminaAttackCost;
}

string Enemy::longDescription() {
    return "Enemy: " + name + ", Health: " + to_string(health) + ", Stamina: " + to_string(stamina) + ", Attack Strength: " + to_string(damage);
}

void Enemy::setStaminaAttackCost(int staminaAttackCost) {
    this->staminaAttackCost = staminaAttackCost;
}

int Enemy::getDamage() {
    return damage;
}

void Enemy::setDamage(int damage) {
    this->damage = damage;
}

string Enemy::getStatus() {
    string health = to_string(getHealth());
    string stamina = to_string(getStamina());
    string ret = "Enemy: ";
    ret += getName() + ", HP: ";
    ret += health + " Stamina: ";
    ret += stamina;
    return ret;
}

Potion* Enemy::getPotion() const {
    return this->potion;
}

void Enemy::setPotion(Potion *potion) {
    this->potion = potion;
}
