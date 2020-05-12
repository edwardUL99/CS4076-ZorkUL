#include "healthboost.h"
#include <sstream>

                                                                             // 9. Initialiser list
HealthBoost::HealthBoost(string description, int boostValue, ZorkUL *game) : Boost(description, boostValue, game) {
}

void HealthBoost::apply() {
    Character &player = game->getPlayer();
    player.setHealth(player.getHealth() + value);
}

string HealthBoost::getLongDescription() {
    stringstream ret;
    ret << "Health Boost: " << description;
    return ret.str();
}
