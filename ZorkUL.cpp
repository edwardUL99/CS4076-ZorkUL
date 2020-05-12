#include <iostream>
#include <map>
#include "ZorkUL.h"
#include "tool.h"
#include "staminaboost.h"
#include "healthboost.h"
#include "pointboost.h"
#include "experienceboost.h"
#include "weapon.h"
#include "potion.h"
using namespace std;

                                       // 9. Initialiser list
ZorkUL::ZorkUL(string characterName) : player(characterName), mobileEnemy("Centaur", 20, 30), staticEnemy("Grue", 15, 20), itemManager(this) {
    playable = true;
	createRooms();
    createItems();

    this->itemManager.spawnUnlockedItems();
    spawn();
}

ZorkUL::~ZorkUL() {
    for (ObjectContainer<Room*>::iterator room = rooms.begin(); room != rooms.end(); room++) {
        delete *room;
    }
}

void ZorkUL::reset() {
    this->playable = true;
    player.setCurrentRoom(getRoom("A"));
    player.itemsInCharacter.clear();
    player.setWeaponInHand(NULL);
    player.setExperience(0);
    player.setLevel(1);

    //clear items in rooms
    spawn(true);
    itemManager.resetSpawned();
    itemManager.spawnUnlockedItems();
    mobileEnemy.setHealth(100);
    mobileEnemy.setStamina(100);
    staticEnemy.setHealth(100);
    staticEnemy.setStamina(100);
}

void ZorkUL::createRooms()  {
    Room *a, *b, *c, *d, *e, *f, *g, *h, *i, *j;

    a = new Room("A");
    rooms + a;
    b = new Room("B");
    rooms + b;
    c = new Room("C");
    rooms + c;
    d = new Room("D");
    rooms + d;
    e = new Room("E");
    rooms + e;
    f = new Room("F");
    rooms + f;
    g = new Room("G");
    rooms + g;
    h = new Room("H");
    rooms + h;
    i = new Room("I");
    rooms + i;
    j = new Room("J");
        j->setLocked(true);
    rooms + j;

//             (N, E, S, W)
    a->setExits(f, b, d, c);
    b->setExits(NULL, NULL, NULL, a);
    c->setExits(NULL, a, NULL, NULL);
    d->setExits(a, e, NULL, i);
    e->setExits(NULL, NULL, NULL, d);
    f->setExits(NULL, g, a, h);
    g->setExits(NULL, NULL, NULL, f);
    h->setExits(NULL, f, NULL, NULL);
    i->setExits(NULL, d, j, NULL);
    j->setExits(i, NULL, NULL, NULL);

    player.setCurrentRoom(a);
}

void ZorkUL::createItems() {
    //revise xp values and higher ranges
    Weapon *arrow = new Weapon("Arrow", 90, 10, 20, 50);
    arrow->setAmmoItem(true);
    Weapon *bow = new Weapon("Bow", 200, 10, 30, 45, arrow);
    Weapon *bullet = new Weapon("Bullet", 20, 1, 20, 50);
    bullet->setAmmoItem(true);
    Weapon *gun = new Weapon("Gun", 500, 10, 40, 25, bullet);

    // 10. Static Dispatch

    //LEVEL 1 ITEMS
    this->itemManager.addItem(new Tool("Key", 100, 5), 1);
    this->itemManager.addItem(new HealthBoost("+5 health", 5, this), 1);
    this->itemManager.addItem(new StaminaBoost("+5 stamina", 5, this), 1);
    this->itemManager.addItem(new PointBoost("+20 points", 20, this), 1);
    this->itemManager.addItem(new ExperienceBoost("+5 XP", 5, this), 1);
    this->itemManager.addItem(new Potion("Extra Fitness", 50, STAMINA_POTION, -5, 10000), 1);
    this->itemManager.addItem(new Potion("Weaken Enemies", 50, SPECIAL_EFFECT, 0, 10000), 1);
    this->itemManager.addItem(new Weapon("Baseball Bat", 100, 10, 25, 45), 1);
    this->itemManager.addItem(new Tool("Candle", 50, 5), 1); //doesn't last

    //LEVEL 2 ITEMS
    this->itemManager.addItem(bow, 2);
    this->itemManager.addItem(arrow, 2);
    this->itemManager.addItem(new HealthBoost("+10 health", 10, this), 2);
    this->itemManager.addItem(new StaminaBoost("+10 stamina", 10, this), 2);
    this->itemManager.addItem(new PointBoost("+40 points", 40, this), 2);
    this->itemManager.addItem(new ExperienceBoost("+10 XP", 10, this), 2);
    this->itemManager.addItem(new Potion("Increased Strength", 50, ATTACK_POTION, 20, 10000), 2);
    this->itemManager.addItem(new Potion("Freeze Enemies", 50, SPECIAL_EFFECT, 0, 10000), 2);
    this->itemManager.addItem(new Tool("Lantern", 75, 10), 2); //doesnt last

    //LEVEL 3 ITEMS
    this->itemManager.addItem(new Weapon("Sword", 100, 10, 35, 30), 3);
    this->itemManager.addItem(new Tool("Emerald", 50, 20), 3);
    this->itemManager.addItem(new HealthBoost("+25 health", 25, this), 3);
    this->itemManager.addItem(new StaminaBoost("+25 stamina", 25, this), 3);
    this->itemManager.addItem(new ExperienceBoost("+15 XP", 15, this), 3);
    this->itemManager.addItem(new PointBoost("+100 points", 100, this), 3);
    this->itemManager.addItem(new Tool("Torch", 90, 15), 3);

    //LEVEL 4 ITEMS
    this->itemManager.addItem(gun, 4);
    this->itemManager.addItem(bullet, 4);
    this->itemManager.addItem(new HealthBoost("+50 health", 50, this), 4);
    this->itemManager.addItem(new StaminaBoost("+50 stamina", 50, this), 4);
    this->itemManager.addItem(new PointBoost("+200 points", 200, this), 4);

    //Miscellaneous items
    this->itemManager.addEnemyPotion(new Potion("Stamina Curse", 0, STAMINA_POTION, 10, 10000), staticEnemy);
    this->itemManager.addItem(new Tool("Lucky Charm", 50, 0), 0, true);
}

Room* ZorkUL::getRoom(string room) {
    for (int i = 0; i < rooms.size(); i++) {
        if (rooms[i]->getName().compare(room) == 0) {
            return rooms[i];
        }
    }
    return NULL;
}

bool ZorkUL::isPlayable() const {
    return playable;
}

void ZorkUL::setPlayable(bool playable) {
    this->playable = playable;
}

string ZorkUL::printHelp() {
    string ret = "";
    ret += "Controls:\n";
    ret += "Press the corresponding buttons to go a certain direction, teleport for a random room\nTo pick up an item, select it from the drop-down list\nTo view and interact with collected items, press Inventory\n";
    ret += "To view the current map, press Map\n";
    ret += "To attack an enemy, press the attack button:\n";
    ret += "    The attack will, by default, use punch or the last picked up weapon, but you can choose a weapon in inventory screen\n";
    ret += "Please note the following:\n";
    ret += "    If you crash into a wall, you lose health\n   Moving also uses stamina (teleport more than using direction buttons)\n   After certain periods of time your health and stamina restores\nTo win: You win if you kill all enemies";
    ret += "\n  In a dark room you will need a light source to see/collect items";
    return ret;
}

bool ZorkUL::enemyOccupied(string roomName) {
    string staticRoom = staticEnemy.getCurrentRoom() != NULL ? staticEnemy.getCurrentRoom()->getName():"";
    string mobileRoom = mobileEnemy.getCurrentRoom() != NULL ? mobileEnemy.getCurrentRoom()->getName():"";
    return staticRoom == roomName || mobileRoom == roomName;
}

string ZorkUL::map()
{
    bool run = true;

    Room *currentRoom = player.getCurrentRoom();

    string rooms[10] = {"H", "F", "G", "C", "A", "B", "I" , "D", "E", "J"};
    int index = 0;
    int hyphenIndex = 0, row = 0;

    string map = "Subtitle: [player] !enemy!\n\n";
    string room;

    while (run) {
        room = rooms[index];
        int nextIndex = index + 1 < 10 ? index + 1: 9;
        bool currRoom = false;
        bool enemy = false;
        bool nextCurrRoom = false;
        if (currentRoom->getName() == room) {
            room = "[" + room + "]";
            currRoom = true;
        } else if (enemyOccupied(room)) {
            room = "!" + room + "!";
            enemy = true;
        }

        if (rooms[nextIndex] == currentRoom->getName()) {
            nextCurrRoom = true;
        }

        //Lines below are all formatting code so the map does not appear too skewed if there are [] or !! around the room
        if (hyphenIndex <= 1 && (room != "J" && room != "!J!" && room != "[J]")) {
            if (nextCurrRoom) { //if this room doesn't have the player in it but the next one does, only put 1 space in the hyphen so the map isn't pushed forward too much
                nextCurrRoom = false;
                if (!enemy) {
                     if (hyphenIndex == 0) map += room + " -";
                    else map += room + "- ";
                } else {
                    enemy = false;
                    map += room + "-";
                }
            } else if (currRoom) {
                if (hyphenIndex == 0) map += room + "-";
                else map+= room + "- ";
            } else {
                if (!enemy) {
                    if (hyphenIndex == 0) map += room + " -";
                    else map += room + "- ";
                } else {
                    enemy = false;
                    map += room + "-";
                }
            }
            hyphenIndex++;
        } else if (room != "J" && room != "!J!" && room != "[J]") { //maybe here use row as an indicator
            string newLine = row <= 1 ? "\n    |     \n":"";
            map += room + newLine;
            hyphenIndex = 0;
            row++;
        }  else if (room == "J" || room == "!J!" || room == "[J]") {
            map += "\n|\n";
            map += room;
            run = false;
        }
        index++;
    }
    return map;
}

string ZorkUL::goToRandomRoom() {
    Room* nextRoom = rooms.getRandomObject();
    while (nextRoom == player.getCurrentRoom()) {
        nextRoom = rooms.getRandomObject();
    }
    if (!nextRoom->isLocked()) player.setCurrentRoom(nextRoom);

    return nextRoom->getName(); //maybe extract this code to its own method as you're duplicating
}

string ZorkUL::go(string direction) {
    Room* nextRoom = player.getCurrentRoom()->getExit(direction);

    if (nextRoom == NULL) {
		return("direction null");
    } else
    {
        if (!nextRoom->isLocked()) player.setCurrentRoom(nextRoom);
        return nextRoom->getName();
	}
}

void ZorkUL::spawn(bool respawn) {
    if (respawn) {
        for (Room* r : rooms) {
            r->clearItems();
        }
    }

    mobileEnemy.setDamage((rand() % 20) + 20); //have different attack strengths each game
    staticEnemy.setDamage((rand() % 20) + 10);

    Room* dark1 = rooms.getRandomObject();
    Room* dark2 = rooms.getRandomObject();

    bool changeA = (dark1->getName() == "A" && player.getCurrentRoom() == dark1) || (dark2->getName() == "A" && player.getCurrentRoom() == dark2);

    while (dark1 == dark2 || changeA) {
        dark1 = rooms.getRandomObject();
        dark2 = rooms.getRandomObject();
        changeA = (dark1->getName() == "A" && player.getCurrentRoom() == dark1) || (dark2->getName() == "A" && player.getCurrentRoom() == dark2);
    }
    dark1->setDark(true);
    dark2->setDark(true);

    Room* mobileEnemyRoom = rooms.getRandomObject();
    Room* staticEnemyRoom = rand() % 2 == 0 ? dark1:dark2;

    while (mobileEnemyRoom == staticEnemyRoom) {
        mobileEnemyRoom = rooms.getRandomObject();
    }

    mobileEnemy.setCurrentRoom(mobileEnemyRoom);
    staticEnemy.setCurrentRoom(staticEnemyRoom);
}

Character &ZorkUL::getPlayer() {
    return player;
}

Enemy &ZorkUL::getEnemy(bool staticEnemy) {
    if (staticEnemy) {
        return this->staticEnemy;
    } else {
        return this->mobileEnemy;
    }
}

void ZorkUL::moveMobileEnemy() {
    if (mobileEnemy.getHealth() != 0 && mobileEnemy.getCurrentRoom() != player.getCurrentRoom()) {
        mobileEnemy.setCurrentRoom(adjacentRoomsToMobileEnemy().getRandomObject());
    }
}

ObjectContainer<Room*> ZorkUL::adjacentRoomsToMobileEnemy() {
    ObjectContainer<Room*> adjacent;
    Room* current = mobileEnemy.getCurrentRoom();

    Room* west = current->getExit("west");
    if (west) {
        adjacent + west;
    }

    Room* north = current->getExit("north");
    if (north) {
        adjacent + north;
    }

    Room* east = current->getExit("east");
    if (east) {
        adjacent + east;
    }

    Room* south = current->getExit("south");
    if (south) {
        adjacent + south;
    }

    return adjacent;
}

int ZorkUL::computePlayerAttackCost() {
    int attack = 0;

    if (player.getWeaponInHand()) {
        attack = player.getWeaponInHand()->getAttackStrength();
    } else {
        attack = 20; //punch
    }

    for (auto it = player.itemsInCharacter.begin(); it != player.itemsInCharacter.end(); it++) {
        Item* item = *it;
        Potion* potion = Item::cast<Potion*>(item);
        if (potion) {
            if (potion->getType() == ATTACK_POTION) attack += potion->getPotionBonus();
        }
    }

    return attack;
}

int ZorkUL::computeEnemyAttackCost(Enemy &enemy) {
    int attack = enemy.getDamage();
    Item* weakenPotion = itemManager.getItem("Weaken Enemies");
    bool weakenEffect = player.hasItem(weakenPotion) && weakenPotion;

    if (weakenEffect) {
        attack -= (attack * 0.4);
    }

    if (enemy.getStamina() == 0) {
        attack = 0;
    }

    return attack;
}

string ZorkUL::attack() {
    string message = "";
    Room* currentRoom = player.getCurrentRoom();

    if (playable) {
        bool mobileInRoom = currentRoom == mobileEnemy.getCurrentRoom(), staticInRoom = currentRoom == staticEnemy.getCurrentRoom();
        Weapon *weapon = player.getWeaponInHand();
        int attack = this->computePlayerAttackCost();
        bool canAttack = true;

        if (weapon) {
            Weapon *ammo = weapon->getAmmo();
            bool needsAmmo = ammo != NULL;

            if (needsAmmo) {
                canAttack = player.hasItem(ammo);
                if (!canAttack) {
                    message = "You need : " + ammo->getShortDescription() + " to use: " + weapon->getShortDescription();
                }
            }

            if (mobileInRoom) {
                int specialAttack = rand();
                if (specialAttack % 3 == 0) {
                    //throw the weapon out of player's hands
                    itemManager.spawnItem(weapon, currentRoom);
                    itemManager.dropItem(weapon);
                    player.setWeaponInHand(NULL);

                    message = "Centaur has knocked the weapon out of your hand";
                } else if (specialAttack % 5 == 0) {
                    //destroy the weapon
                    itemManager.dropItem(weapon);
                    weapon->setDamage(0);

                    message = "Centaur has destroyed your weapon";
                }
            }
        }

        canAttack = canAttack && (player.getStamina() != 0 || player.hasItem("Emerald"));

        if (mobileInRoom && canAttack)
            attackEnemy(mobileEnemy, weapon, attack);

        if (staticInRoom && canAttack)
            attackEnemy(staticEnemy, weapon, attack);
    }

    return message;
}

void ZorkUL::attackEnemy(Enemy &enemy, Weapon *weapon, int attack) {
    enemy.setHealth(enemy.getHealth() - attack);

    if (enemy.getHealth() > 0 && enemy.getStamina() > 0) {
        enemy.setStamina(enemy.getStamina() - enemy.getStaminaAttackCost());
    } else if (enemy.getHealth() <= 0) {
        enemy.setHealth(0);
        enemy.setStamina(0);
    }

    if (weapon) {
        (*weapon)--;
    }

    player.setStamina(player.getStamina() - player.getMoveStaminaCost());
}

void ZorkUL::attackPlayer(Enemy &enemy) {
    int damage = this->computeEnemyAttackCost(enemy);
    player.setHealth(player.getHealth() - damage);
    if (player.getHealth() == 0) emit player.gameLost();
}

Item* ZorkUL::getItem(string name) {
    for (auto it = itemManager.itemsIterator(true); it != itemManager.itemsIterator(false); it++) {
        if ((*it)->getShortDescription() == name) {
            return *it;
        }
    }
    return NULL;
}
