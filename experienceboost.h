#ifndef EXPERIENCEBOOST_H
#define EXPERIENCEBOOST_H
#include "boost.h"

// 2. Inheritance (and virtual functions) and cascading constructors
class ExperienceBoost : public Boost {
public:
    ExperienceBoost(string description, int boostValue, ZorkUL *game);
    // 1. Destructors
    virtual ~ExperienceBoost() = default;
    // 7. Virtual functions and polymorphism
    virtual void apply() override;
    virtual string getLongDescription() override;
};

#endif // EXPERIENCEBOOST_H
