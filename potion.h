#ifndef POTION_H
#define POTION_H
#include "item.h"
#include <string>
using namespace std;

enum PotionType {
    ATTACK_POTION,
    STAMINA_POTION,
    SPECIAL_EFFECT
};

// 2. Inheritance (including virtual methods) and cascading constructors
class Potion : public Item {
private:
    PotionType type;
    int potionBonus; //the amount "extra" to add to attack strength or the ammount "less" to cost stamina
    int timeOutTime;

public:
    Potion(string description, int value, PotionType type, int potionBonus, int timeOutTime);
    // 1. Destructors
    virtual ~Potion() = default;

    // 7. Virtual Functions and polymorphism
    virtual string getLongDescription() override;
    PotionType getType() const;
    void setType(PotionType type);
    int getPotionBonus() const;
    void setPotionBonus(int potionBonus);
    int getTimeOutTime() const;
    void setTimeOutTime(int timeOutTime);
};

#endif // POTION_H
