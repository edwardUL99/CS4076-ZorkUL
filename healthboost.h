#ifndef HEALTHBOOST_H
#define HEALTHBOOST_H

#include "boost.h"

// 2.Inheritance (including virtual methods) and cascading constructors
class HealthBoost : public Boost {
public:
    HealthBoost(string description, int boostValue, ZorkUL *game);
    //1. Destructors
    virtual ~HealthBoost() = default;
    void apply() override;
    // 7. Virtual functions and polymorphism
    virtual string getLongDescription() override;
};

#endif // HEALTHBOOST_H
