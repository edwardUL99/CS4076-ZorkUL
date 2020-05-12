#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include "objectcontainer.h"
#include "item.h"
using namespace std;

class Room {

private:
    string name;
    map<string, Room*> exits;
	string exitString();
    ObjectContainer<Item*> itemsInRoom;
    bool locked;
    bool dark;
    void removeItemFromRoom(Item* item);
    void addItem(Item *inItem);
    string displayItem(bool playerHasLight);

public:
    int numberOfItems();
    Room(string name);
    // 1.Destructors
    ~Room();
	void setExits(Room *north, Room *east, Room *south, Room *west);
    string getName();
    string longDescription(bool playerHasLight);
    Room* getExit(string direction);
    void clearItems();
    Item* getItem(string description);
    int isItemInRoom(string inString);
    bool isLocked() const;
    void setLocked(bool locked);
    //if a room is dark and the player doesn't have a light, the player loses health because they are hitting off things because they cannot see
    bool isDark() const;
    void setDark(bool dark);
    ObjectContainer<Item*>::iterator itemsIterator(bool beginEnd);
    int numberOfItemsInRoom();
    //6. Friends
    friend class ItemManager; //only allow Item Manager access to add or remove items
};

#endif
