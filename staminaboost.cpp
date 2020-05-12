#include "staminaboost.h"
#include <sstream>

                                                                               // 9. Initialiser list
StaminaBoost::StaminaBoost(string description, int boostValue, ZorkUL *game) : Boost(description, boostValue, game) {
}

void StaminaBoost::apply() {
    Character &player = game->getPlayer();
    player.setStamina(player.getStamina() + value);
}

string StaminaBoost::getLongDescription() {
    stringstream ret;
    ret << "Stamina Boost: " << description;
    return ret.str();
}
