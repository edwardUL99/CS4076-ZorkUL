#include "experienceboost.h"
#include <sstream>

                                                                                    // 9. Initializer lists
ExperienceBoost::ExperienceBoost(string description, int boostValue, ZorkUL *game) : Boost(description, boostValue, game) {}

void ExperienceBoost::apply() {
    Character &player = game->getPlayer();
    player.setExperience(player.getExperience() + value);
}

string ExperienceBoost::getLongDescription() {
    stringstream s;
    s << "Experience Boost: +" << to_string(value) << " XP";
    return s.str();
}
