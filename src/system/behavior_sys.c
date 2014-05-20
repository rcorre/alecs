#include "system/behavior_sys.h"

static double elapsed_time;
static const double angle_close_enough = PI / 32;

static void update_behavior(ecs_component *comp) {
  ecs_entity *ent = comp->owner_entity;
  Behavior b = comp->behavior;
  // try to move towards target
  ecs_component *propulsion_comp = ent->components[ECS_COMPONENT_PROPULSION];
  if (propulsion_comp) {
    Propulsion *p = &propulsion_comp->propulsion;
    p->linear_throttle = (vector){1, 0};
    // displacement to target
    vector disp = vector_sub(b.target->position, ent->position);
    // angle to target
    double angle_dif = angle_between(ent->angle, vector_angle(disp));
    double throttle = fmin(1, fabs(angle_dif) / (p->turn_rate * elapsed_time));
    p->angular_throttle = angle_dif > 0 ? throttle : -throttle;
  }
}

void behavior_system_fn(double time) {
  elapsed_time = time;
  list *components = ecs_component_store[ECS_COMPONENT_BEHAVIOR];
  list_each(components, (list_lambda)update_behavior);
}
