#ifndef STAMINABOOST_H
#define STAMINABOOST_H

#include "boost.h"

// 2. Inheritance (including virtual methods) and cascading constructors */
class StaminaBoost : public Boost {
public:
    StaminaBoost(string description, int boostValue, ZorkUL *game);
    // 1. Destructors
    virtual ~StaminaBoost() = default;
    void apply() override;
    // 7. Virtual functions and polymorphism
    virtual string getLongDescription() override;
};

#endif // STAMINABOOST_H
