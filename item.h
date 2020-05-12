#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <string>
#include <iostream>
using namespace std;


class Potion;
class Boost;

class Item {
protected:
    string description;
    int value;
    int xpValue;
    bool dropped; //if the item was dropped and picked up again, don't give the points back because the player already got the points for it
    bool timed; //if timed is true, it is currently on a timer loop

public:
    Item (string description, int value, int xpValue);
    Item (string description);
    /***1. Destructors */
    virtual ~Item() = default;
    string getShortDescription();
    // 8. Abstract Classes and pure virtual functions
    virtual string getLongDescription() = 0; //description depends on the type of item so have different types implement it
    int getValue() const;
    void setValue(int value);
    int getXpValue() const;
    void setXpValue(int xpValue);
    bool isDropped() const;
    void setDropped(bool dropped);
    bool isTimed() const;
    void setTimed(bool timed);

    // 3. Templates
    template <typename E>
    static E cast(Item* item) {
        return dynamic_cast<E>(item);
    } //casts this item to the specified sub item
};

#endif /*ITEM_H_*/
