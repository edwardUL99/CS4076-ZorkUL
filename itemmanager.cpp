#include "itemmanager.h"
#include "item.h"
#include "tool.h"
#include "staminaboost.h"
#include "healthboost.h"
#include "pointboost.h"
#include "weapon.h"
#include "potion.h"

#include "ZorkUL.h"

ItemManager::ItemManager(ZorkUL *game) {
    this->game = game;
}

ItemManager::~ItemManager() {
    for (auto it = items.begin(); it != items.end(); it++) {
        delete *it;
    }
}

void ItemManager::addItem(Item *item, int level, bool hideFromInventory) {
    this->items + item;
    this->itemLevels[item] = level;
    this->spawn[item] = true;
    this->hideInv[item] = hideFromInventory;
    this->pickedUp[item] = false;
}

void ItemManager::addEnemyPotion(Potion *potion, Enemy &enemy) {
    this->items + potion;
    this->spawn[potion] = false;
    this->hideInv[potion] = true;
    enemy.setPotion(potion);
}

void ItemManager::removeItem(Item *item, bool roomOnly) {
    if (spawnedRooms[item] != NULL) {
        spawnedRooms[item]->removeItemFromRoom(item);

        if (roomOnly) {
            return;
        }

        items - item;

        itemLevels.erase(item);
        spawnedRooms.erase(item);
        spawn.erase(item);

        delete item;
    }
}

void ItemManager::pickUpItem(Item *item) {
    if (items.contains(item)) {
        //can only pickUp an item that exists to the item manager
        pickedUp[item] = true;
    }
}

void ItemManager::dropItem(Item *item) {
    pickedUp[item] = false;
}

Item* ItemManager::getItem(string description) {
    for (auto it = items.begin(); it != items.end(); it++) {
        if ((*it)->getShortDescription() == description) {
            return *it;
        }
    }

    return NULL;
}

bool ItemManager::showInInventory(Item *item) {
    return !hideInv[item];
}

string ItemManager::getItemLevelDescription(Item *item) {
    int level = itemLevels[item];
    string desc = item->getLongDescription();
    desc += ", Level: " + to_string(level);
    bool unlocked = this->isItemUnlocked(item);
    string status = unlocked ? "Unlocked":"Locked";
    desc += ", Status: " + status;
    return desc;
}

void ItemManager::spawnItem(Item *item) {
    Room *room = game->rooms.getRandomObject();
    string description = item->getShortDescription();
    if (description == "Key") {
        while (room->isLocked() || room->isDark()) {
            room = game->rooms.getRandomObject();
        }
    } else if (description == "Torch" || description == "Lantern" || description == "Candle") {
        while (room->isDark()) {
            room = game->rooms.getRandomObject();
        }
    }
    room->addItem(item);
    spawnedRooms[item] = room;
}

void ItemManager::spawnItem(string name) {
    spawnItem(getItem(name));
}

void ItemManager::respawnItem(Item *item) {
    spawnedRooms[item] = NULL;
    spawnItem(item);
}

void ItemManager::spawnItem(Item *item, Room *room) {
    room->addItem(item);
    spawnedRooms[item] = room;
}

bool ItemManager::spawnItems(bool xp) {
    Character &player = game->getPlayer();
    int level = player.getLevel();
    bool newItems = false;

    for (Item *item : items) {
        int requiredLevel = xp ? this->itemLevels[item]:level;
        if (level >= requiredLevel && spawnedRooms[item] == NULL && spawn[item]) {
            this->spawnItem(item);
            newItems = true;
        }
    }

    return newItems;
}

bool ItemManager::spawnUnlockedItems() {
    return this->spawnItems(true);
}

bool ItemManager::spawnAllItems() {
    return this->spawnItems(false);
}

void ItemManager::resetSpawned() {
    this->spawnedRooms.clear();
}

ObjectContainer<Item*> ItemManager::spawnedItems(bool includePickedUp) {
    ObjectContainer<Item*> spawned;
    for (auto it = items.begin(); it != items.end(); it++) {
        Item *item = *it;
        bool add = this->spawnedRooms[item] != NULL;

        if (!includePickedUp) {
            add = add && !pickedUp[item];
        }

        if (this->spawnedRooms[item]) {
            spawned + item;
        }
    }

    return spawned;
}

int ItemManager::getItemLevel(Item *item) {
    return itemLevels[item];
}

bool ItemManager::isItemUnlocked(Item *item) {
    int level = game->getPlayer().getLevel();

    return level >= itemLevels[item];
}

ObjectContainer<Item*>::iterator ItemManager::itemsIterator(bool begin) {
    if (begin) {
        return items.begin();
    } else {
        return items.end();
    }
}
