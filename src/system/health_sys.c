#include "system/health_sys.h"

static double elapsed_time;

static void update_health(ecs_component *health_comp) {
  Health *health = &health_comp->health;
  if (health->hp <= 0) { // invoke disable delegate
    health->on_disable(health_comp->owner_entity);
    // set delegate to null so it is only called once
    health->on_disable = NULL;
  }
  particle_generator *gen = &health->particle_effect;
  if (gen->data) { // update health particle effect
    ecs_entity *ent = health_comp->owner_entity;
    gen->position = ent->position;
    ecs_component *body_comp = ent->components[ECS_COMPONENT_BODY];
    vector src_vel = body_comp ? body_comp->body.velocity : ZEROVEC;
    double density = 1 - health->hp / health->max_hp;
    spawn_particles(gen, elapsed_time, density, src_vel);
  }
}

void health_system_fn(double time) {
  elapsed_time = time;
  list *components = ecs_component_store[ECS_COMPONENT_HEALTH];
  list_each(components, (list_lambda)update_health);
}

void deal_damage(struct ecs_entity *entity, double amount) {
  ecs_component *comp = entity->components[ECS_COMPONENT_HEALTH];
  if (comp) {  // make sure entity has a health component
    Health *health = &comp->health;
    health->hp -= amount;
  }
}

Health make_health(double hp, ecs_entity_trigger on_disable,
    char *particle_effect_name)
{
  Health health;
  health.hp = health.max_hp = hp;
  health.on_disable = on_disable;
  if (particle_effect_name) {
    health.particle_effect = get_particle_generator(particle_effect_name);
    health.particle_effect.angle = -PI / 2; // point upwards
  }
  else {
    health.particle_effect.data = NULL;
  }
  return health;
}
