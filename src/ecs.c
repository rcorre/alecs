#include "ecs.h"


ecs_component_manager ecs_component_managers[NUM_COMPONENT_TYPES];
list *ecs_component_store[NUM_COMPONENT_TYPES];
ecs_system ecs_systems[NUM_SYSTEM_TYPES];

void ecs_init() {
}
ecs_entity *ecs_create_entity();
void ecs_destroy_entity(ecs_entity *entity);
void ecs_attach_component(ecs_entity *entity, struct ecs_component *template);
struct ecs_component ecs_get_component_template(ecs_component_type type, 
    const char *name);

