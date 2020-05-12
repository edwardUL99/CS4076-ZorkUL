#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <QObject>

#include <string>
using namespace std;
#include "objectcontainer.h"

#include "item.h"
#include "weapon.h"
#include "Room.h"

class Character : public QObject {
    Q_OBJECT
private:
    Room *currentRoom;
    ObjectContainer<Item*> itemsInCharacter;
    Weapon* weaponInHand;
    bool key;

protected:
    string name;
    int health; //there will be a timer restoring health after every timeout of a specified interval
    int stamina; //just like health there will be a timer for stamina
    int points;
    int experience;
    int level;
    const int healthCost; //the amount to decrease health by
    static const int originalMoveStaminaCost;
    int moveStaminaCost; //the amount to decrease stamina by after a move west/east/north/south/attack
    const int teleportStaminaCost; //the amount to decrease stamina by after each teleport

public:
    Character(string name);
    /***1. Destructors */
    ~Character();
    string getName();
    // 7. Virtual functions
    virtual string longDescription();
    Room* getCurrentRoom();
    void setCurrentRoom(Room* room);
    void addItem(Item *item);
    void removeItem(Item *item);
    bool hasItem(Item *item);
    bool hasItem(string name);
    bool hasKey() const;
    void setKey(bool hasKey);
    bool hasLight();
    int getPoints() const;
    void setPoints(int points);
    int getHealth() const;
    void setHealth(int health);
    int getStamina() const;
    void setStamina(int stamina);
    int getExperience() const;
    void setExperience(int experience);
    int getLevel() const;
    void setLevel(int level);
    void checkStatus(); //checks status and if health is 0, you lose
    string getStatus();
    void setMoveStaminaCost(int moveStaminaCost);
    int getMoveStaminaCost() const;
    Weapon* getWeaponInHand();
    void setWeaponInHand(Weapon* weapon);
    /*** 6. Friends */
    friend class MainWindow;
    friend class ZorkUL;

signals:
    void gameWon(); //you win the game by defeating all enemies
    void gameLost(); //you lose the game if you run out of health
};

#endif /*CHARACTER_H_*/
