#ifndef ZORKUL_H_
#define ZORKUL_H_

#include "Room.h"
#include "item.h"
#include "Character.h"
#include "enemy.h"
#include <iostream>
#include <string>
#include "objectcontainer.h"
#include "itemmanager.h"
using namespace std;

class ZorkUL {
private:
    Character player;
    Enemy mobileEnemy;
    Enemy staticEnemy;
    ObjectContainer<Room*> rooms;
    bool playable;
    ItemManager itemManager;
	void createRooms();
    void createItems();
    Room* getRoom(string room);
    string printHelp();
    string goToRandomRoom();
    void spawn(bool respawn = false); //spawn enemies and items here, spawn key potion curse etc too
    bool enemyOccupied(string roomName);
    int computePlayerAttackCost();
    int computeEnemyAttackCost(Enemy &enemy);
    ObjectContainer<Room*> adjacentRoomsToMobileEnemy();

public:
    ZorkUL(string characterName);
    /***1. Destructors */
    ~ZorkUL();
    void reset();
    void play();
    string go(string direction);
    string map();
    bool isPlayable() const;
    void setPlayable(bool playable);
    Character &getPlayer();
    Enemy &getEnemy(bool staticEnemy);
    Item* getItem(string name);
    void moveMobileEnemy();
    string attack(); //if something happened after the attack, a string message is returned
    void attackPlayer(Enemy &enemy);
    void attackEnemy(Enemy &enemy, Weapon *weapon, int attack);
    // 6.friends
    friend class MainWindow;
    friend class ItemManager;
    friend class InventoryWindow;
};

#endif /*ZORKUL_H_*/
