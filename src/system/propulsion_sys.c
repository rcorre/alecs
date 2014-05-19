#include "system/propulsion_sys.h"

static void propulsion_update(ecs_component *comp);

static double elapsed_time;

void propulsion_system_fn(double time) {
  elapsed_time = time;
  list *propulsion_list = ecs_component_store[ECS_COMPONENT_PROPULSION];
  list_each(propulsion_list, (list_lambda)propulsion_update);
}

static void propulsion_update(ecs_component *comp) {
  assert(comp->type == ECS_COMPONENT_PROPULSION);
  assert(comp->owner_entity != NULL);
  assert(comp->owner_entity->components[ECS_COMPONENT_BODY] != NULL);
  Propulsion p = comp->propulsion;
  Body *b = &comp->owner_entity->components[ECS_COMPONENT_BODY]->body;
  vector dxy = vector_scale(p.linear_throttle, p.linear_accel * elapsed_time);
  double rotation =  p.angular_accel * p.angular_throttle * elapsed_time;
  b->velocity = vector_add(b->velocity, dxy);
  b->angular_velocity += rotation;
  // handle particle effect
  if (p.particle_effect.data != NULL) {
    p.particle_effect.angle = PI - comp->owner_entity->angle;
    p.particle_effect.position = comp->owner_entity->position;
    spawn_particles(&p.particle_effect, elapsed_time, 1.0, b->velocity);
  }
}
