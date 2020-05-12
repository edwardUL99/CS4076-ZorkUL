#ifndef POINTBOOST_H
#define POINTBOOST_H

#include "boost.h"

// 2. Inheritance (including virtual methods) and cascading constructors
class PointBoost : public Boost {
public:
    PointBoost(string description, int boostValue, ZorkUL *game);
    //1. Destructors
    virtual ~PointBoost() = default;
    void apply() override;
    // 7. Virtual functions and polymorphism
    virtual string getLongDescription() override;
};

#endif // POINTBOOST_H
