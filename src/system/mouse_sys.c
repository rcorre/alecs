#include "system/mouse_sys.h"

static bool lmb_down, rmb_down;
static point prev_mouse_pos = {-1, -1};

void ecs_handle_mouse(ALLEGRO_EVENT ev) {
  ALLEGRO_MOUSE_EVENT mouse = ev.mouse;
  point mousepos = {mouse.x, mouse.y};
  switch (ev.type) {
    case ALLEGRO_EVENT_MOUSE_AXES:
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      if (mouse.button == 0) { lmb_down = true; }
      if (mouse.button == 1) { rmb_down = true; }
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      if (mouse.button == 0) { lmb_down = false; }
      if (mouse.button == 1) { rmb_down = false; }
      break;
    default:
      return;
  }
  list *components = ecs_component_store[(int)ECS_COMPONENT_MOUSE_LISTENER];
  list_node *node = components->head;
  for (; node != NULL ; node = node->next) {
    ecs_component *comp = (ecs_component*)node->value;
    MouseListener *listener = &comp->mouse_listener;
    struct ecs_entity *ent = comp->owner_entity;
    assert(comp->type == ECS_COMPONENT_MOUSE_LISTENER);
    // set mouse detection rect
    listener->click_rect.x = ent->position.x - listener->click_rect.w / 2;
    listener->click_rect.y = ent->position.y - listener->click_rect.h / 2;
    // check if mouse just entered listener
    if (listener->on_enter != NULL &&
        rect_contains_point(listener->click_rect, mousepos) &&
        !rect_contains_point(listener->click_rect, prev_mouse_pos))
    {
      listener->on_enter(ent);
    }
    else if (listener->on_leave != NULL &&
        !rect_contains_point(listener->click_rect, mousepos) &&
        rect_contains_point(listener->click_rect, prev_mouse_pos))
    {
      listener->on_leave(ent);
    }
  }
  prev_mouse_pos = mousepos;
}
