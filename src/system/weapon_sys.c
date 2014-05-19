#include "system/weapon_sys.h"

static ecs_entity *current_target;
static list *lockon_list;

void weapon_system_fn(double time) {
}

void weapon_set_target(ecs_entity *target) {
  current_target = target;
}
