#ifndef ENEMY_H
#define ENEMY_H
#include <string>
#include <QObject>
#include "Character.h"
using namespace std;

class Potion;

// 2. Inheritance and cascading constructors*/
class Enemy : public Character
{
private:
    int staminaAttackCost;
    int damage; //the amount of damage the enemy inflicts on the player
    Potion* potion;

public:
    Enemy(string name, int staminaAttackCost, int damage, Potion *potion = NULL);
    // 1.Destructors
    virtual ~Enemy() = default;
    //7. Virtual functions
    virtual string longDescription() override;
    int getStaminaAttackCost();
    void setStaminaAttackCost(int staminaAttackCost);
    int getDamage();
    void setDamage(int damage);
    string getStatus();
    bool getPotionAttack() const;
    void setPotionAttack(bool potionAttack);
    Potion* getPotion() const;
    void setPotion(Potion *potion);
};

#endif // ENEMY_H
