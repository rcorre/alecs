#ifndef PLAYER_H
#define PLAYER_H

#include "ecs.h"
#include "system/weapon_sys.h"

ecs_entity* make_player_ship();

// Weapons
extern Weapon seeker_launcher;
extern Weapon destroyer_launcher;
extern Weapon swarmer_launcher;

#endif /* end of include guard: PLAYER_H */
