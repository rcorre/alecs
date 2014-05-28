#include "system/propulsion_sys.h"

static void propulsion_update(ecs_component *comp);

static double elapsed_time;

void propulsion_system_fn(double time) {
  elapsed_time = time;
  list *propulsion_list = ecs_component_store[ECS_COMPONENT_PROPULSION];
  list_node *node = propulsion_list->head;
  while (node) {
    ecs_component *comp = node->value;
    if (comp->active) {
      propulsion_update(comp);
      node = node->next;
    }
    else {
      node = list_remove(propulsion_list, node, free);
    }
  }
}

static void propulsion_update(ecs_component *comp) {
  assert(comp->type == ECS_COMPONENT_PROPULSION);
  assert(comp->owner_entity != NULL);
  assert(comp->owner_entity->components[ECS_COMPONENT_BODY] != NULL);
  Propulsion p = comp->propulsion;
  Body *b = &comp->owner_entity->components[ECS_COMPONENT_BODY]->body;
  vector dxy = vector_scale(p.linear_throttle, p.linear_accel * elapsed_time);
  if (p.directed) {
    dxy = vector_rotate(dxy, comp->owner_entity->angle);
  }
  b->velocity = vector_add(b->velocity, dxy);
  comp->owner_entity->angle += p.turn_rate * p.angular_throttle * elapsed_time;
  comp->owner_entity->angle = normalize_angle(comp->owner_entity->angle);
  // handle particle effect
  if (p.particle_effect.data != NULL) {
    //spawn in opposite direction of entity
    p.particle_effect.angle = comp->owner_entity->angle + PI;
    p.particle_effect.position = comp->owner_entity->position;
    spawn_particles(&p.particle_effect, elapsed_time, 1.0, b->velocity);
  }
}

static void propulsion_stop_sample(ecs_component *comp) {
  al_stop_sample(&comp->propulsion._sample_id);
}

void propulsion_assign_sound(struct ecs_entity *prop_entity, char *sound_name) {
  ecs_component *prop_comp = prop_entity->components[ECS_COMPONENT_PROPULSION];
  Propulsion *prop = &prop_comp->propulsion;
  prop->_sample_id = al_game_play_sound(sound_name, true);
  prop_comp->on_destroy = propulsion_stop_sample;
}
