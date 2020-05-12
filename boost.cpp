#include "boost.h"

                                                                 // 9. Initialiser list
Boost::Boost(string description, int boostValue, ZorkUL *game) : Item(description, boostValue, 0) {
    this->game = game;
}
