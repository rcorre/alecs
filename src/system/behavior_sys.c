#include "system/behavior_sys.h"

// if distance to target is less than this, consider it reached
const static double close_enough = 5;
static double elapsed_time;

static void move_toward(ecs_entity *ent, Propulsion *p, vector target) {
  // displacement to target
  vector disp = vector_sub(target, ent->position);
  if (p->directed) { // adjust angle towards target to move towards it
    p->linear_throttle = (vector){1, 0};
    // angle to target
    double angle_dif = angle_between(ent->angle, vector_angle(disp));
    double throttle = fmin(1, fabs(angle_dif) / (p->turn_rate * elapsed_time));
    p->angular_throttle = angle_dif > 0 ? throttle : -throttle;
  }
  else { // keep angle the same while moving directly towards target
    p->linear_throttle = vector_norm(disp);
  }
}

static void update_behavior(ecs_component *comp) {
  ecs_entity *ent = comp->owner_entity;
  Behavior b = comp->behavior;
  // try to move towards target
  ecs_component *propulsion_comp = ent->components[ECS_COMPONENT_PROPULSION];
  assert(propulsion_comp);
  Propulsion *p = &propulsion_comp->propulsion;
  if (b.type == BEHAVIOR_FOLLOW) {
    move_toward(ent, p, b.target->position);
  }
  else if (b.type == BEHAVIOR_MOVE) {
    // displacement from current to desired location
    move_toward(ent, p, b.location);
    if (vector_dist(ent->position, b.location) < close_enough) {
      p->linear_throttle = ZEROVEC;
      p->angular_throttle = 0;
      Body *bod = &ent->components[ECS_COMPONENT_BODY]->body;
      // TODO: this is cheating, behavior shouldn't directly modify velocity
      bod->velocity = ZEROVEC;
      b.type = BEHAVIOR_IDLE;
    }
  }
}

void behavior_system_fn(double time) {
  elapsed_time = time;
  list *components = ecs_component_store[ECS_COMPONENT_BEHAVIOR];
  list_node *node = components->head;
  while (node) {
    ecs_component *comp = node->value;
    if (comp->active) {
      update_behavior(comp);
      node = node->next;
    }
    else {
      node = list_remove(components, node, free);
    }
  }
}
