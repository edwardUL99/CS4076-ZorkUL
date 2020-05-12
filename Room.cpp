#include "Room.h"

Room::Room(string name) {
    this->name = name;
    this->locked = false;
    this->dark = false;
}

Room::~Room() {
}

void Room::setExits(Room *north, Room *east, Room *south, Room *west) {
	if (north != NULL)
        exits["north"] = north;
    if (east != NULL)
        exits["east"] = east;
    if (south != NULL)
        exits["south"] = south;
    if (west != NULL)
        exits["west"] = west;
}

string Room::getName() {
    return name;
}

string Room::longDescription(bool playerHasLight) {
    return "room = " + name + ".\n" + displayItem(playerHasLight) + exitString();
}

string Room::exitString() {
	string returnString = "\nexits =";
    for (map<string, Room*>::iterator i = exits.begin(); i != exits.end(); i++)
		// Loop through map
		returnString += "  " + i->first;	// access the "first" element of the pair (direction as a string)
	return returnString;
}

Room* Room::getExit(string direction) {
    map<string, Room*>::iterator next = exits.find(direction); //returns an iterator for the "pair"
	if (next == exits.end())
		return NULL; // if exits.end() was returned, there's no room in that direction.
	return next->second; // If there is a room, remove the "second" (Room*)
				// part of the "pair" (<string, Room*>) and return it.
}

void Room::addItem(Item *inItem) {
    //cout <<endl;
    //cout << "Just added" + inItem->getLongDescription();
    itemsInRoom.addObject(inItem);
}

void Room::clearItems() {
    itemsInRoom.clear();
}

Item* Room::getItem(string description) {
    for (int i = 0; i < itemsInRoom.size(); i++) {
        if (itemsInRoom[i]->getShortDescription().compare(description) == 0) {
            Item* it = itemsInRoom[i];
            return it;
        }
    }
    return NULL;
}

void Room::removeItemFromRoom(Item *item) {
    itemsInRoom - item;
}

string Room::displayItem(bool playerHasLight) {
    string tempString = "items in room = ";
    int sizeItems = (itemsInRoom.size());
    bool canSee = !dark || playerHasLight;
    if (itemsInRoom.size() < 1) {
        tempString = "no items in room";
    } else if (itemsInRoom.size() > 0 && canSee) {
       int x = (0);
        for (int n = sizeItems; n > 0; n--) {
            tempString = tempString + itemsInRoom[x]->getShortDescription() + "  " ;
            x++;
        }
    } else {
        tempString += "it is too dark to see the items";
    }
    return tempString;
}

int Room::numberOfItems() {
    return itemsInRoom.size();
}

int Room::isItemInRoom(string inString)
{
    int sizeItems = (itemsInRoom.size());
    if (itemsInRoom.size() < 1) {
        return false;
        }
    else if (itemsInRoom.size() > 0) {
       int x = (0);
        for (int n = sizeItems; n > 0; n--) {
            // compare inString with short description
            int tempFlag = inString.compare( itemsInRoom[x]->getShortDescription());
            if (tempFlag == 0) {
                itemsInRoom.removeObject(itemsInRoom.begin()+x);
                return x;
            }
            x++;
            }
        }
    return -1;
}

ObjectContainer<Item*>::iterator Room::itemsIterator(bool beginEnd) {
    if (beginEnd) return itemsInRoom.begin();
    else return itemsInRoom.end();
}

int Room::numberOfItemsInRoom() {
    return itemsInRoom.size();
}

bool Room::isLocked() const {
    return locked;
}

void Room::setLocked(bool locked) {
    this->locked = locked;
}

bool Room::isDark() const {
    return dark;
}

void Room::setDark(bool dark) {
    this->dark = dark;
}
