#include "system/health_sys.h"

static void update_health(ecs_component *health_comp) {
  Health *health = &health_comp->health;
  if (health->hp <= 0) { // invoke disable delegate
    health->on_disable(health_comp->owner_entity);
    // set delegate to null so it is only called once
    health->on_disable = NULL;
  }
}

void health_system_fn(double time) {
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
