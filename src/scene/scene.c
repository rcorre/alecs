#include "scene/scene.h"

static scene current_scene = {0};

void register_scene(scene s) {
  scene_shutdown();
  current_scene = s;
}

void scene_handle_input(ALLEGRO_EVENT ev) {
  switch(ev.type) {
    case ALLEGRO_EVENT_KEY_DOWN:
    case ALLEGRO_EVENT_KEY_UP:
      if (current_scene.keyboard != NULL) {
        current_scene.keyboard(ev.keyboard);
      }
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
    case ALLEGRO_EVENT_MOUSE_AXES:
      if (current_scene.mouse != NULL) {
        current_scene.mouse(ev.mouse);
      }
      break;
  }
}

bool scene_update(double time) {
  if (current_scene.update != NULL) {
    return current_scene.update(time);
  }
  return false;
}

void scene_draw() {
  if (current_scene.draw != NULL) {
    current_scene.draw();
  }
}

void scene_shutdown() {
  if (current_scene.shutdown != NULL) {
    current_scene.shutdown();
  }
  current_scene = (scene){0};
}
