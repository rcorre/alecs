#include "system/keyboard_sys.h"

void ecs_handle_keypress(ALLEGRO_EVENT ev) {
  bool down;
  switch (ev.type) {
    case ALLEGRO_EVENT_KEY_DOWN:
      down = true;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      down = false;
      break;
    default:
      return;
  }
  list *components = ecs_component_store[(int)ECS_COMPONENT_KEYBOARD_LISTENER];
  list_node *node = components->head;
  while (node) {
    ecs_component *comp = node->value;
    if (comp->active) {
      assert(comp->type == ECS_COMPONENT_KEYBOARD_LISTENER);
      ecs_keyboard_handler handler = comp->keyboard_listener.handler;
      assert(handler != NULL);
      handler(comp->owner_entity, ev.keyboard.keycode, down);
      node = node->next;
    }
    else {
      node = list_remove(components, node, free);
    }
  }
}
