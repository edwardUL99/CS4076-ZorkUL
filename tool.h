#ifndef ARTEFACT_H
#define ARTEFACT_H

#include "item.h"

// 2. Inheritance (including virtual methods) and cascading constructors */
class Tool : public Item {
public:
    Tool(string description, int value, int xpValue);
    /**1. destructors */
    virtual ~Tool() = default;
    // 7. Virtual functions and polymorphism
    virtual string getLongDescription() override;
};

#endif // ARTEFACT_H
