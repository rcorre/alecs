#include "system/timer_sys.h"

static double elapsed_time;

static void update_timer(ecs_component *timer_comp) {
  Timer *timer = &timer_comp->timer;
  ecs_entity *ent = timer_comp->owner_entity;
  timer->time_left -= elapsed_time;
  if (timer->time_left < 0) {
    timer->timer_action(ent);
  }
}

void timer_system_fn(double time) {
  elapsed_time = time;
  list *timers = ecs_component_store[ECS_COMPONENT_TIMER];
  list_node *node = timers->head;
  while (node) {
    ecs_component *comp = node->value;
    if (comp->active) {
      update_timer(comp);
      node = node->next;
    }
    else {
      node = list_remove(timers, node, free);
    }
  }
}
