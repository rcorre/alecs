#ifndef SCENE_H
#define SCENE_H

#include <allegro5/allegro.h>

// event processing functions
typedef void (*mouse_handler)(ALLEGRO_MOUSE_EVENT);
typedef void (*keyboard_handler)(ALLEGRO_KEYBOARD_EVENT);
typedef bool (*update_handler)(double);
typedef void (*draw_handler)(void);
typedef void (*shutdown_handler)(void);

typedef struct scene {
  mouse_handler mouse;
  keyboard_handler keyboard;
  update_handler update;
  draw_handler draw;
  shutdown_handler shutdown;
} scene;

// set the current scene
void register_scene(scene s);
// ask current scene to handle mouse or keyboard input
void scene_handle_input(ALLEGRO_EVENT ev);
// update current scene with elapsed time. Returns false to request termination
bool scene_update(double time);
// draw current scene to display
void scene_draw();
// call current scene's shutdown function and set all handlers to NULL
void scene_shutdown();

#endif /* end of include guard: INPUT_MANAGER_H */

