#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "objectcontainer.h"

// 2. Inheritance (including virtual methods) and cascading constructors */
class Weapon : public Item {
private:
    int attackStrength; //the amount of damage it can do
    Weapon *ammo; //need this "ammo" item to work
    int damage; //the amount of damage the weapon can take, if it is 0 it is destroyed
    int damageCost; //the lower the number the more durable the weapon
    bool ammoItem;

public:
    Weapon(string description, int value, int xpValue, int attackStrength, int damageCost, Weapon *ammo = NULL);
    // 1. Destructors
    virtual ~Weapon();
    // 2. Virtual functions
    virtual string getLongDescription() override;
    Weapon* getAmmo();
    void setAmmo(Weapon *ammo);
    bool isAmmoItem();
    void setAmmoItem(bool ammoItem);
    int getAttackStrength();
    void setAttackStrength(int strength);
    int getDamage();
    void setDamage(int damage);
    int getDamageCost();
    void setDamageCost(int damageCost);
    // 4. Unary operator overloading
    void operator--(int); //postfix reduces weapons damage level
};

#endif // WEAPON_H
