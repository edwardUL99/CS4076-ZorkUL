#include "pointboost.h"
#include <sstream>

                                                                           // 9. Initialiser list
PointBoost::PointBoost(string description, int boostValue, ZorkUL *game) : Boost(description, boostValue, game) {
}

void PointBoost::apply() {
    Character &player = game->getPlayer();
    player.setPoints(player.getPoints() + value);
}

string PointBoost::getLongDescription() {
    stringstream ret;
    ret << "Point Boost: " << description;
    return ret.str();
}
