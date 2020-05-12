#ifndef BOOST_H
#define BOOST_H

#include "item.h"
#include "ZorkUL.h"

// 2. Inheritance (including virtual methods) and cascading constructors
class Boost : public Item {
protected:
    ZorkUL *game;

public:
    Boost(string description, int boostValue, ZorkUL *game);
    // 1. Destructors
    virtual ~Boost() = default;
    // 8.Abstract classes and pure virtual functions
    virtual void apply() = 0; //applies the boost to the current player in MainWindow //need to implement
};

#endif // BOOST_H
