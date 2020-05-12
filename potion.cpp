#include "potion.h"

                                                                                                   // 9.Initializer list
Potion::Potion(string description, int value, PotionType type, int potionBonus, int timeOutTime) : Item(description, value, 0) {
    this->type = type;
    this->potionBonus = potionBonus;
    this->timeOutTime = timeOutTime;
}

string Potion::getLongDescription() {
    string ret = "Potion: ";
    ret += description + ", Type: ";
    string type = "";
    if (this->type == STAMINA_POTION) {
        type = "Stamina";
    } else if (this->type == ATTACK_POTION) {
        type = "Attack";
    } else {
        type = "Special Effect";
    }

    ret += type;

    if (this->type != SPECIAL_EFFECT) {
        ret += ", Bonus: ";
        string bonus = this->type == ATTACK_POTION ? "+ " + to_string(potionBonus) + " attack strength":to_string(potionBonus) + " stamina movement cost";
        ret += bonus;
    }
    return ret;
}

PotionType Potion::getType() const {
    return type;
}

void Potion::setType(PotionType type) {
    this->type = type;
}

int Potion::getPotionBonus() const {
    return potionBonus;
}

void Potion::setPotionBonus(int potionBonus) {
    this->potionBonus = potionBonus;
}

int Potion::getTimeOutTime() const {
    return timeOutTime;
}

void Potion::setTimeOutTime(int timeOutTime) {
    this->timeOutTime = timeOutTime;
}
