#include "Character.h"
#include "boost.h"
#include "potion.h"

const int Character::originalMoveStaminaCost(10);

/*** 9. Initialiser list */
Character::Character(string name) : healthCost(25), moveStaminaCost(Character::originalMoveStaminaCost), teleportStaminaCost(20) {
    this->name = name;
    this->health = 100;
    this->stamina = 100;
    this->points = 0;
    this->experience = 0;
    this->level = 1;
    this->currentRoom = NULL;
    this->weaponInHand = NULL;
    this->key = false;
}

Character::~Character() {
}

string Character::getName() {
    return name;
}

void Character::addItem(Item *item) {
    itemsInCharacter.addObject(item);

    Boost *boost = Item::cast<Boost*>(item);
    Potion *potion = Item::cast<Potion*>(item);
    if (!item->isDropped() && !boost && !potion) this->points += item->getValue();
}

void Character::removeItem(Item *item) {
    itemsInCharacter - item;
}

bool Character::hasItem(Item *item) {
    return itemsInCharacter.contains(item);
}

bool Character::hasItem(string name) {
    for (ObjectContainer<Item*>::iterator it = itemsInCharacter.begin(); it != itemsInCharacter.end(); it++) {
        if ((*it)->getShortDescription() == name) {
            return true;
        }
    }
    return false;
}

string Character::longDescription()
{
  string ret = this->name;
  ret += "\n Item list:\n";
  for (ObjectContainer<Item*>::iterator i = itemsInCharacter.begin(); i != itemsInCharacter.end(); i++)
    ret += "\t"+ (*(*i)).getLongDescription() + "\n";
  return ret;
}

void Character::checkStatus() {
    if (health == 0) {
        emit gameLost();
    }
}

bool Character::hasKey() const {
    return key;
}

void Character::setKey(bool hasKey) {
    this->key = hasKey;
}

bool Character::hasLight() {
    for (ObjectContainer<Item*>::iterator it = itemsInCharacter.begin(); it != itemsInCharacter.end(); it++) {
        Item *item = *it;
        string description = item->getShortDescription();
        if (description == "Torch" || description == "Lantern" || description == "Candle") {
            return true;
        }
    }

    return false;
}

int Character::getPoints() const {
    return this->points;
}

void Character::setPoints(int points) {
    this->points = points;
}

int Character::getHealth() const {
    return this->health;
}

void Character::setHealth(int health) {
    this->health = health;
    if (this->health > 100) this->health = 100;
    else if (this->health < 0) this->health = 0;
}

int Character::getStamina() const {
    return this->stamina;
}

void Character::setStamina(int stamina) {
    this->stamina = stamina;
    if (this->stamina > 100) this->stamina = 100;
    if (this->stamina < 0) this->stamina = 0;
}

int Character::getExperience() const {
    return this->experience;
}

void Character::setExperience(int experience) {
    this->experience = experience;
}

int Character::getLevel() const {
    return this->level;
}

void Character::setLevel(int level) {
    this->level = level;
}

Room* Character::getCurrentRoom() {
    return currentRoom;
}

void Character::setCurrentRoom(Room *room) {
    this->currentRoom = room;
}

void Character::setMoveStaminaCost(int moveStaminaCost) {
    this->moveStaminaCost = moveStaminaCost;
}

int Character::getMoveStaminaCost() const {
    return moveStaminaCost;
}

string Character::getStatus() {
    string health = to_string(getHealth());
    string stamina = to_string(getStamina());
    string ret = "Player: ";
    ret += getName() + ", HP: ";
    ret += health + " Stamina: ";
    ret += stamina;
    return ret;
}

Weapon* Character::getWeaponInHand() {
    return weaponInHand;
}

void Character::setWeaponInHand(Weapon *weapon) {
    this->weaponInHand = weapon;
}



