#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H
#include "objectcontainer.h"
#include <map>
using namespace std;

class ZorkUL;
class Item;
class Potion;
class Room;
class Enemy;

/**
 *ItemManager
 * @brief The ItemManager class
 * Class to manage all aspects related to items that will be spawned in the game
 * Controls the spawning of items in terms of player experience and level,
 * i.e. provides the functionality to restrict certain items spawning only if the player's level is above the minimum level
 * Keeps track of the room each item is spawned in, allowing quick removal of each item from the room and the game
 * Also manages destruction of each item. This is IMPORTANT, do not delete items anywhere else
 */
class ItemManager {
private:
    ZorkUL *game;
    ObjectContainer<Item*> items;
    map<Item*, int> itemLevels; //the levels each item are spawned at
    map<Item*, Room*> spawnedRooms; //maps an item to the room it was spawned in
    map<Item*, bool> spawn; //map to determine if an item should be spawned in the game world
    map<Item*, bool> hideInv; //map to hide item from inventory
    map<Item*, bool> pickedUp;
    bool spawnItems(bool xp);

public:
    ItemManager(ZorkUL *game = nullptr);
    // 1. Destructors
    ~ItemManager();
    //level is 0 if it is to spawn regardless of experience, show means to show it in inventory
    void addItem(Item* item, int level, bool hideFromInventory = false); //adds items, but DOESN'T spawn them if the current level is >= level, level is the level at which to unlock the item
    void addEnemyPotion(Potion* potion, Enemy &enemy); //adds an item belonging to the enemy, i.e. does not need a level
    void removeItem(Item *item, bool roomOnly = false); //if roomOnly it means, only to remove it from the room the item is contained in, not the manager
    void pickUpItem(Item *item); //mark the item as picked up
    void dropItem(Item *item); //mark the item as dropped
    Item* getItem(string description);
    bool showInInventory(Item *item);
    bool spawnUnlockedItems();
    bool spawnAllItems();
    //these three methods spawn randomly
    void spawnItem(Item* item);
    void spawnItem(string item);
    void respawnItem(Item *item);
    //spawn in a specified room
    void spawnItem(Item* item, Room* room);
    void resetSpawned();
    int getItemLevel(Item *item);
    string getItemLevelDescription(Item* item);
    bool isItemUnlocked(Item *item);
    ObjectContainer<Item*> spawnedItems(bool includePickedUp); //include items picked up by player if true
    ObjectContainer<Item*>::iterator itemsIterator(bool begin);
};

#endif // ITEMMANAGER_H
