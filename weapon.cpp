#include "weapon.h"
#include <sstream>

                                                                                                                             // 9. Initialiser list
Weapon::Weapon(string description, int value, int xpValue, int attackStrength, int damageCost, Weapon *ammo) : Item(description, value, xpValue) {
    this->attackStrength = attackStrength;
    this->damage = 100;
    this->damageCost = damageCost;
    this->ammo = ammo;
    this->ammoItem = false;
}

Weapon::~Weapon() {}

string Weapon::getLongDescription() {
    stringstream ret;
    string name = isAmmoItem() ? "Ammo: ":"Weapon: ";
    ret << name << description << ", Value: " << value << ", Attack Strength: " << attackStrength;
    return ret.str();
}

Weapon* Weapon::getAmmo() {
    return ammo;
}

void Weapon::setAmmo(Weapon *ammo) {
    if (!ammoItem) this->ammo = ammo;
}

bool Weapon::isAmmoItem() {
    return ammoItem;
}

void Weapon::setAmmoItem(bool ammoItem) {
    this->ammoItem = ammoItem;
}

int Weapon::getAttackStrength() {
    return attackStrength;
}

void Weapon::setAttackStrength(int attackStrength) {
    this->attackStrength = attackStrength;
}

int Weapon::getDamage() {
    return damage;
}

void Weapon::setDamage(int damage) {
    this->damage = damage;
    if (this->damage < 0) {
        this->damage = 0;
    }
}

int Weapon::getDamageCost() {
    return damageCost;
}

void Weapon::setDamageCost(int damageCost) {
    this->damageCost = damageCost;
}

void Weapon::operator--(int) {
    this->damage -= this->damageCost;
    if (this->damage < 0) this->damage = 0;
}
