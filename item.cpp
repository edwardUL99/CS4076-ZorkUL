#include "item.h"
#include <sstream>

Item::Item (string inDescription, int inValue, int xpValue) {
    this->description = inDescription;
    this->value = inValue;
    this->dropped = false;
    this->xpValue = xpValue;
}

Item::Item(string inDescription) {
	description = inDescription;
}

void Item::setValue(int inValue)
{
	   value = inValue;
}

int Item::getValue() const {
    return value;
}

int Item::getXpValue() const {
    return this->xpValue;
}

void Item::setXpValue(int xpValue) {
    this->xpValue = xpValue;
}

string Item::getShortDescription()
{
	return description;
}

string Item::getLongDescription() {
    stringstream ret;
    ret << "Item: " << this->description << ", Value: " << to_string(this->value);
    return ret.str();
}

bool Item::isDropped() const {
    return dropped;
}

void Item::setDropped(bool dropped) {
    this->dropped = dropped;
}

bool Item::isTimed() const {
    return timed;
}

void Item::setTimed(bool timed) {
    this->timed = timed;
}

